/*
 * Copyright (C) 2009-2011 Christopho, Solarus - http://www.solarus-engine.org
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
#include "entities/Hero.h"
#include "entities/MapEntities.h"
#include "entities/Boomerang.h"
#include "hero/HeroSprites.h"
#include "Game.h"
#include "Map.h"
#include "Savegame.h"
#include "Equipment.h"
#include "lowlevel/Debug.h"
#include "lowlevel/StringConcat.h"
#include <lua.hpp>

/**
 * @brief Makes the hero go into a scriptable state.
 *
 * - Argument 1 (string): name of the state
 *
 * @param l the Lua context that is calling this function
 */
int Script::hero_api_change_state(lua_State* l) {

  Script& script = get_script(l, 1);

  const std::string& scriptname = luaL_checkstring(l, 1);

  script.get_game().get_hero().start_scriptable(scriptname);

  return 0;
}

/**
 * @brief Makes the hero go into a scriptable state.
 *
 * @param l the Lua context that is calling this function
 */
int Script::hero_api_change_state_free(lua_State* l) {

  Script& script = get_script(l, 0);

  script.get_game().get_hero().start_free();

  return 0;
}

/**
 * @brief Makes the hero go into a scriptable state.
 *
 * @param l the Lua context that is calling this function
 */
int Script::hero_api_get_wanted_direction8(lua_State* l) {

    Script& script = get_script(l, 0);

    lua_pushinteger(l, script.get_game().get_controls().get_wanted_direction8());

    return 1;
}

/**
 * @brief Makes the hero go into a scriptable state.
 *
 * @param l the Lua context that is calling this function
 */
int Script::hero_api_spawn_entity(lua_State* l) {

    Script& script = get_script(l, 4);

    const int max_distance = luaL_checkinteger(l, 1);
    const int speed = luaL_checkinteger(l, 2);
    const int angle = luaL_checkinteger(l, 3);
    const std::string& sprite_name = luaL_checkstring(l, 4);

    script.get_game().get_hero().get_map().get_entities().add_entity(new Boomerang(script.get_game().get_hero(), max_distance, speed, angle, sprite_name));

    return 0;

}

/**
 * @brief Sets the hero's animation to a custom sprite
 *
 * - Argument 1 (string): tunic animation
 * - Argument 2 (string): shield animation
 *
 * @param l the Lua context that is calling this function
 */
int Script::hero_api_set_animation_custom(lua_State* l) {

  Script& script = get_script(l, 2);

  const std::string& anim_tunic = luaL_checkstring(l, 1);
  const std::string& anim_shield = luaL_checkstring(l, 2);

  script.get_game().get_hero().get_sprites().set_animation(anim_tunic, anim_shield);

  return 0;
}


/**
 * @brief Return whether the hero's current animation is finished
 *
 * - Argument 1 (string): tunic animation
 * - Argument 2 (string): shield animation
 *
 * @param l the Lua context that is calling this function
 */
int Script::hero_api_is_animation_finished(lua_State* l) {

  Script& script = get_script(l, 0);
  
  lua_pushboolean(l, script.get_game().get_hero().is_animation_finished());

  return 1;
}
