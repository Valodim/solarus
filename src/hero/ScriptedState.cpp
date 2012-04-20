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
#include "hero/ScriptedState.h"
#include "hero/FreeState.h"
#include "hero/HeroSprites.h"
#include "entities/MapEntities.h"
#include "lowlevel/Geometry.h"
#include "lua/StateScript.h"
#include "Game.h"
#include "Map.h"

/**
 * @brief Constructor.
 * @param hero the hero controlled by this state
 * @param max_distance maximum distance of the movement in pixels
 * @param speed speed of the movement in pixels per second
 * @param tunic_preparing_animation animation name of the hero's tunic sprite
 * when preparing the boomerang
 * @param sprite_name animation set id that represents the boomerang
 */
Hero::ScriptedState::ScriptedState(Hero& hero, std::string scriptname):
  State(hero) {
      script = new StateScript(hero.get_game(), scriptname);
}

/**
 * @brief Destructor.
 */
Hero::ScriptedState::~ScriptedState() {
    delete script;
}

/**
 * @brief Starts this state.
 * @param previous_state the previous state
 */
void Hero::ScriptedState::start(State* previous_state) {

  State::start(previous_state);

  script->event_start();

}

/**
 * @brief Updates this state.
 */
void Hero::ScriptedState::update() {

  State::update();

  script->update();
}

/**
 * @brief Updates this state.
 */
void Hero::ScriptedState::event_update() {

  State::update();

}
