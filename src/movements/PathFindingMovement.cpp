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
#include "movements/PathFindingMovement.h"
#include "movements/PathFinding.h"
#include "lua/Script.h"
#include "entities/MapEntity.h"
#include "lowlevel/Random.h"
#include "lowlevel/System.h"
#include "lowlevel/Debug.h"
#include "lowlevel/StringConcat.h"
#include <sstream>

/**
 * @brief Creates a chase movement.
 * @param speed speed of the movement in pixels per second
 */
PathFindingMovement::PathFindingMovement(int speed):
  PathMovement("", speed, false, false, true),
  target(NULL) {

}

/**
 * @brief Destructor.
 */
PathFindingMovement::~PathFindingMovement() {

}

/**
 * @brief Sets the entity to target with this movement.
 */
void PathFindingMovement::set_target(MapEntity& target) {

  this->target = &target;
  next_recomputation_date = System::now() + 100;
}

/**
 * @brief Updates the position.
 */
void PathFindingMovement::update() {

  PathMovement::update();

  if (is_suspended()) {
    return;
  }

  if (PathMovement::is_finished()) {

    // there was a collision or the path was made
    if (target != NULL
        && System::now() >= next_recomputation_date
        && get_entity()->is_aligned_to_grid()) {
      recompute_movement();
    }
    else {
      set_path(create_random_path());
    }
  } 
}

/**
 * @brief Calculates the direction and the speed of the movement
 * depending on the target.
 */
void PathFindingMovement::recompute_movement() { 

  PathFinding path_finding(get_entity()->get_map(), *get_entity(), *target);
  std::string path = path_finding.compute_path();

  uint32_t min_delay;
  if (path.size() == 0) {
    // the target is too far or there is no path
    path = create_random_path();

    // no path was found: no need to try again very soon
    // (note that the A* algorithm is very costly when it explores all nodes without finding a solution)
    min_delay = 3000;
  }
  else {
    // a path was found: we need to update it frequently (and the A* algorithm is much faster in general when there is a solution)
    min_delay = 300;
  }
  // compute a new path every random delay to avoid
  // having all path-finding entities of the map compute a path at the same time
  next_recomputation_date = System::now() + min_delay + Random::get_number(200);

  set_path(path);
}

/**
 * @brief Returns whether the movement is finished.
 * @return always false because the movement is restarted as soon as the path is finished
 * or an obstacle is reached
 */
bool PathFindingMovement::is_finished() {
  return false;
}

/**
 * @brief Returns the name identifying this type in Lua.
 * @return the name identifying this type in Lua
 */
const std::string& PathFindingMovement::get_lua_type_name() const {
  return Script::path_finding_movement_module_name;
}

