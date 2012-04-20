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
#include "lua/StateScript.h"
#include "entities/PickableItem.h"
#include "movements/FallingHeight.h"
#include "ItemProperties.h"
#include "InventoryItem.h"
#include "Game.h"
#include "Treasure.h"
#include "hero/State.h"
#include "lowlevel/Debug.h"
#include "lowlevel/StringConcat.h"
#include <lua.hpp>

/**
 * @brief Constructor.
 * @param game the game
 * @param item_properties the static properties of the item
 */
StateScript::StateScript(Game &game, std::string scriptname):
  Script(MAIN_API | GAME_API | MAP_API | ITEM_API),
  game(game) {

  std::string script_name = (std::string) "hero/" + scriptname;
  load_if_exists(script_name);
}

/**
 * @brief Destructor.
 */
StateScript::~StateScript() {

}

/**
 * @brief Returns the game that runs this script.
 * @return the game
 */
Game& StateScript::get_game() {
  return game;
}

/**
 * @brief Returns the current map of the game.
 * @return the map
 */
Map& StateScript::get_map() {
  return game.get_current_map();
}

/**
 * @brief Updates the script.
 */
void StateScript::update() {

  Script::update();

  if (is_loaded()) {
    notify_script("event_update");
  }
}

/**
 * @brief This function is called when the game is being suspended or resumed.
 * @param suspended true if the game is suspended, false if it is resumed
 */
void StateScript::set_suspended(bool suspended) {

  Script::set_suspended(suspended);

  if (is_loaded()) {
    notify_script("event_set_suspended", "i", suspended);
  }
}

void StateScript::event_start() {
    notify_script("event_start");
}
