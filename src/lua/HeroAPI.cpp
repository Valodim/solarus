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
