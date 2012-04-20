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
#include "entities/ScriptedEntity.h"
#include "entities/CustomEnemy.h"
#include "entities/Hero.h"
#include "entities/MapEntities.h"
#include "entities/CarriedItem.h"
#include "entities/PickableItem.h"
#include "entities/DestructibleItem.h"
#include "entities/Fire.h"
#include "Game.h"
#include "Savegame.h"
#include "Equipment.h"
#include "ItemProperties.h"
#include "Sprite.h"
#include "SpriteAnimationSet.h"
#include "Map.h"
#include "lua/MapScript.h"
#include "movements/StraightMovement.h"
#include "movements/FallingHeight.h"
#include "lowlevel/Geometry.h"
#include "lowlevel/FileTools.h"
#include "lowlevel/Random.h"
#include "lowlevel/System.h"
#include "lowlevel/Debug.h"
#include "lowlevel/StringConcat.h"
#include "lowlevel/Sound.h"

const std::string ScriptedEntity::obstacle_behavior_names[] = {
  "normal",
  "flying",
  "swimming",
};

ScriptedEntity::ScriptedEntity(int collision_mode, const std::string &name, Layer layer,
    int x, int y, int width, int height):
  Detector(collision_mode, name, layer, x, y, width, height),
  obstacle_behavior(OBSTACLE_BEHAVIOR_NORMAL),
  displayed_in_y_order(true) {
}

/**
 * @brief Destructor.
 */
ScriptedEntity::~ScriptedEntity() {

}

/**
 * @brief Returns the type of entity.
 * @return the type of entity
 */
EntityType ScriptedEntity::get_type() {
  return SCRIPTED;
}

/**
 * @brief Sets the map.
 *
 * Warning: as this function is called when initializing the map,
 * the current map of the game is still the old one.
 *
 * @param map the map
 */
void ScriptedEntity::set_map(Map &map) {

  MapEntity::set_map(map);

  if (is_enabled()) {
    enable_pixel_collisions();
  }
}

/**
 * @brief Notifies this entity that its map has just become active.
 */
void ScriptedEntity::notify_map_started() {

  MapEntity::notify_map_started();

  // give the sprite their initial direction
  int initial_direction = get_direction();
  std::list<Sprite*>::iterator it;
  for (it = get_sprites().begin(); it != get_sprites().end(); it++) {
    (*it)->set_current_direction(initial_direction);
  }
}

/**
 * @brief Notifies this entity that its map has just become active.
 */
void ScriptedEntity::notify_map_opening_transition_finished() {

  Detector::notify_map_opening_transition_finished();

  if (is_enabled()) {
    // restart();
  }
}

/**
 * @brief Returns whether this entity is an obstacle for another one.
 * @param other another entity
 * @return true if this entity is an obstacle for the other one
 */
bool ScriptedEntity::is_obstacle_for(MapEntity& other) {

  return false;
}

/**
 * @brief Returns whether a destructible item is currently considered as an obstacle for this entity.
 * @param destructible_item a destructible item
 * @return true if the destructible item is currently an obstacle this entity
 */
bool ScriptedEntity::is_destructible_item_obstacle(DestructibleItem& destructible_item) {

  // the destructible item is an obstacle unless the enemy is already overlapping it,
  // which is possible with bomb flowers
  if (this->overlaps(destructible_item)) {
    return false;
  }
  return obstacle_behavior != OBSTACLE_BEHAVIOR_FLYING;
}

/**
 * @brief Returns whether a teletransporter is currently considered as an obstacle.
 * @param teletransporter a teletransporter
 * @return true if the teletransporter is currently an obstacle for this entity
 */
bool ScriptedEntity::is_teletransporter_obstacle(Teletransporter& teletransporter) {
  return false;
}

/**
 * @brief Returns whether a deep water tile is currently considered as an obstacle by this entity.
 * @return true if the deep water tiles are currently an obstacle for this entity
 */
bool ScriptedEntity::is_deep_water_obstacle() {
  return obstacle_behavior != OBSTACLE_BEHAVIOR_FLYING
      && obstacle_behavior != OBSTACLE_BEHAVIOR_SWIMMING;
}

/**
 * @brief Returns whether a shallow water tile is currently considered as an obstacle by this entity.
 * @return true if the shallow water tiles are currently an obstacle for this entity
 */
bool ScriptedEntity::is_shallow_water_obstacle() {
  return false;
}

/**
 * @brief Returns whether a hole is currently considered as an obstacle by this entity.
 * @return true if the holes are currently an obstacle for this entity
 */
bool ScriptedEntity::is_hole_obstacle() {
  return obstacle_behavior != OBSTACLE_BEHAVIOR_FLYING;
}

/**
 * @brief Returns whether prickles are currently considered as obstacle by this entity.
 * @return true if prickles are currently obstacle for this entity
 */
bool ScriptedEntity::is_prickle_obstacle() {
  return false;
}

/**
 * @brief Returns whether lava is currently considered as obstacle by this entity.
 * @return true if lava is currently obstacle for this entity
 */
bool ScriptedEntity::is_lava_obstacle() {
  return obstacle_behavior != OBSTACLE_BEHAVIOR_FLYING;
}

/**
 * @brief Returns the current behavior with obstacles for this enemy.
 * @return the behavior with obstacles
 */
ScriptedEntity::ObstacleBehavior ScriptedEntity::get_obstacle_behavior() {
  return obstacle_behavior;
}

/**
 * @brief Sets the behavior with obstacles for this enemy.
 * @param obstacle_behavior the behavior with obstacles
 */
void ScriptedEntity::set_obstacle_behavior(ObstacleBehavior obstacle_behavior) {
  this->obstacle_behavior = obstacle_behavior;
}

/**
 * @brief Returns the current animation of the first sprite of the enemy.
 *
 * This function is useful when the enemy has several sprites.
 *
 * @return name of the current animation of the first sprite
 */
const std::string& ScriptedEntity::get_animation() {

  return get_sprite().get_current_animation();
}

/**
 * @brief Changes the animation of this enemy's sprites.
 *
 * This function is useful when the enemy has several sprites.
 *
 * @param animation name of the animation to set
 */
void ScriptedEntity::set_animation(const std::string &animation) {

  std::list<Sprite*>::iterator it;
  for (it = get_sprites().begin(); it != get_sprites().end(); it++) {
    (*it)->set_current_animation(animation);
  }
}

/**
 * @brief Returns whether this entity has to be displayed in y order.
 *
 * This function returns whether the entity should be displayed above
 * the hero and other entities having this property when it is in front of them.
 *
 * @return true if this type of entity is displayed at the same level as the hero
 */
bool ScriptedEntity::is_displayed_in_y_order() {
  return displayed_in_y_order;
}

/**
 * @brief Updates the enemy.
 */
void ScriptedEntity::update() {

  MapEntity::update();

  if (suspended || !is_enabled()) {
    return;
  }

}

/**
 * @brief Suspends or resumes the entity.
 * @param suspended true to suspend the entity, false to resume it
 */
void ScriptedEntity::set_suspended(bool suspended) {

  Detector::set_suspended(suspended);

}

/**
 * @brief Notifies this entity that it was just enabled or disabled.
 * @param enabled true if the entity is now enabled
 */
void ScriptedEntity::notify_enabled(bool enabled) {

  MapEntity::notify_enabled(enabled);

  if (enabled) {
    // initialize();
  }
}
/**
 * @brief Notifies the enemy that a collision was just detected with another entity
 * @param entity_overlapping the other entity
 * @param collision_mode the collision mode that detected the collision
 */
void ScriptedEntity::notify_collision(MapEntity &entity_overlapping, CollisionMode collision_mode) {

}

/**
 * @brief Notifies this enemy that a pixel-precise collision was just detected with another sprite.
 *
 * This function is called by check_collision(MapEntity*, Sprite*) when another entity's
 * sprite overlaps a sprite of this enemy.
 *
 * @param other_entity the entity overlapping this detector
 * @param other_sprite the sprite of other_entity that is overlapping this detector
 * @param this_sprite the sprite of this detector that is overlapping the other entity's sprite
 */
void ScriptedEntity::notify_collision(MapEntity &other_entity, Sprite &other_sprite, Sprite &this_sprite) {

}

/**
 * @brief This function is called when an explosion's sprite detects a collision
 * with a sprite of this enemy.
 * @param explosion the explosion
 * @param sprite_overlapping the sprite of this enemy that collides with the explosion
 */
void ScriptedEntity::notify_collision_with_explosion(Explosion &explosion, Sprite &sprite_overlapping) {

}

/**
 * @brief Notifies this entity that a sprite of fire
 * detects a pixel-precise collision with a sprite of this entity.
 * @param fire the fire
 * @param sprite_overlapping the sprite of the current entity that collides with the fire
 */
void ScriptedEntity::notify_collision_with_fire(Fire& fire, Sprite& sprite_overlapping) {

}

/**
 * @brief Returns true if the current sprite animation is finished or is looping.
 * @return true if the current sprite animation is finished or is looping 
 */
bool ScriptedEntity::is_sprite_finished_or_looping() {

  Sprite &sprite = get_sprite();
  return sprite.is_animation_finished() || sprite.is_animation_looping();
}

/**
 * @brief Sends a message from another enemy to this enemy.
 * @param sender the sender
 * @param message the message
 */
void ScriptedEntity::notify_message_received(ScriptedEntity& sender, const std::string& message) {

}

/**
 * @brief Converts a value of the ObstacleBehavior enumeration into a string.
 * @param behavior a behavior with obstacles
 * @return the corresponding string
 */
const std::string& ScriptedEntity::get_obstacle_behavior_name(ObstacleBehavior behavior) {

  Debug::check_assertion(behavior >= 0 && behavior < OBSTACLE_BEHAVIOR_NUMBER,
      StringConcat() << "Invalid obstacle behavior number: " << behavior);

  return obstacle_behavior_names[behavior];
}

/**
 * @brief Converts a string into a value of the ObstacleBehavior enumeration.
 * @param name name of a behavior with obstacles
 * @return the corresponding behavior
 */
ScriptedEntity::ObstacleBehavior ScriptedEntity::get_obstacle_behavior_by_name(const std::string& name) {

  for (int i = 0; i < OBSTACLE_BEHAVIOR_NUMBER; i++) {
    if (obstacle_behavior_names[i] == name) {
      return ObstacleBehavior(i);
    }
  }

  Debug::die(StringConcat() << "Invalid obstacle behavior name: " << name);
  throw;
}
