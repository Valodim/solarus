/*
 * Copyright (C) 2006-2012 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "lua/Script.h"
#include "Timer.h"
#include "Game.h"
#include <lua.hpp>

const std::string Script::timer_module_name = "sol.timer";

/**
 * @brief Initializes the timer features provided to Lua.
 */
void Script::register_timer_module() {

  static const luaL_Reg methods[] = {
      { "start", timer_api_start },
      { "stop", timer_api_stop },
      { "stop_all", timer_api_stop_all},
      { NULL, NULL }
  };
  static const luaL_Reg metamethods[] = {
      { "__gc", userdata_meta_gc },
      { NULL, NULL }
  };
  register_type(timer_module_name, methods, metamethods);
}

/**
 * @brief Registers a timer created by this script.
 * @param timer a timer
 * @param callback_ref registry ref of a function to call when the timer
 * finishes
 */
void Script::add_timer(Timer* timer, int callback_ref) {

  set_created(timer);
  timers[timer] = callback_ref;

  if (is_new_timer_suspended()) {
    timer->set_suspended(true);
  }
  increment_refcount(timer);
}

/**
 * @brief Unregisters a timer created by this script.
 * @param timer a timer
 */
void Script::remove_timer(Timer* timer) {

  if (!timer->is_finished()) {
    luaL_unref(l, LUA_REGISTRYINDEX, timers[timer]);
  }
  timers.erase(timer);
  decrement_refcount(timer); // will be deleted if Lua has already collected it
}

/**
 * @brief Unregisters all timers created by this script.
 */
void Script::remove_all_timers() {

  std::map<Timer*, int>::iterator it;
  for (it = timers.begin(); it != timers.end(); it++) {
    Timer* timer = it->first;
    if (!timer->is_finished()) {
      luaL_unref(l, LUA_REGISTRYINDEX, it->second);
    }
    decrement_refcount(timer);
  }
  timers.clear();
}

/**
 * @brief Updates all timers currently running for this script.
 */
void Script::update_timers() {

  std::map<Timer*, int>::iterator it;
  for (it = timers.begin(); it != timers.end(); ++it) {

    Timer* timer = it->first;
    int callback_ref = it->second;

    timer->update();
    if (timer->is_finished()) {
      do_callback(callback_ref);
      remove_timer(timer);
      break;
    }
  }
}

/**
 * @brief Returns whether a timer just created should be initially suspended.
 * @return true to initially suspend a new timer
 */
bool Script::is_new_timer_suspended(void) {

  if (apis_enabled & GAME_API) {
    // start the timer even if the game is suspended (e.g. a timer started during a camera movement)
    // except when it is suspended because of a dialog box
    return get_game().is_showing_dialog();
  }

  return false;
}

/**
 * @brief Checks that the userdata at the specified index of the stack is a
 * timer and returns it.
 * @param l a Lua context
 * @param index an index in the stack
 * @return the timer
 */
Timer& Script::check_timer(lua_State* l, int index) {
  return static_cast<Timer&>(check_userdata(l, index, timer_module_name));
}

/**
 * @brief Pushes a timer userdata onto the stack.
 * @param l a Lua context
 * @param timer a timer
 */
void Script::push_timer(lua_State* l, Timer& timer) {
  push_userdata(l, timer);
}

/**
 * @brief Implementation of \ref lua_api_timer_start.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::timer_api_start(lua_State *l) {

  // parameters: delay [with_sound] callback
  Script& script = get_script(l);
  uint32_t delay = luaL_checkinteger(l, 1);
  bool with_sound = false;
  int index = 2;
  if (lua_isboolean(l, index)) {
    with_sound = lua_toboolean(l, 2);
    index++;
  }
  luaL_checktype(l, index, LUA_TFUNCTION);
  lua_settop(l, index); // make sure the function is on top of the stack

  int callback_ref = luaL_ref(l, LUA_REGISTRYINDEX);
  if (delay > 0) {
    // create the timer
    Timer* timer = new Timer(delay, with_sound);
    script.add_timer(timer, callback_ref);
    push_timer(l, *timer);
  }
  else {
    // delay is zero: call the function right now
    script.do_callback(callback_ref);
  }

  return 0;
}

/**
 * @brief Implementation of \ref lua_api_timer_stop.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::timer_api_stop(lua_State* l) {

  Script& script = get_script(l);
  Timer& timer = check_timer(l, 1);

  script.remove_timer(&timer);

  return 0;
}

/**
 * @brief Implementation of \ref lua_api_timer_stop_all.
 * @param l the Lua context that is calling this function
 * @return number of values to return to Lua
 */
int Script::timer_api_stop_all(lua_State *l) {

  Script& script = get_script(l);
  luaL_checktype(l, 1, LUA_TTABLE);

  // traverse the table
  lua_pushnil(l); // first key
  while (lua_next(l, 1) != 0) {

    Timer& timer = check_timer(l, -1);
    script.remove_timer(&timer);
    lua_pop(l, 1); // pop the value, let the key for the iteration
  }

  return 0;
}

