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
#ifndef SOLARUS_ITEM_SCRIPT_H
#define SOLARUS_ITEM_SCRIPT_H

#include "Common.h"
#include "lua/Script.h"
#include "hero/State.h"

/**
 * @brief Represents the Lua script of a hero state.
 *
 * Such a script only exist in-game since it manages the dynamic behavior of an item.
 * The static properties of a state are handled by the State class.
 * If the Lua file of the state does not exist, this class has no effect.
 *
 * This class makes the interface between the engine C++ code and the Lua script of a hero state.
 */
class StateScript: public Script {

  private:

    Game &game;						/**< the game */
    Hero::State *state;					/**< static properties of the item */

  protected:

    Game &get_game();
    Map &get_map();
    Hero::State *get_state();

  public:

    StateScript(Game &game, std::string);
    ~StateScript();

    void update();
    void set_suspended(bool suspended);

    void event_start();

};

#endif

