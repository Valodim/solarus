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
#ifndef SOLARUS_SCRIPTED_ENTITY_H
#define SOLARUS_SCRIPTED_ENTITY_H

#include "Common.h"
#include "Treasure.h"
#include "entities/Detector.h"
#include "entities/Explosion.h"
#include "lua/EntityScript.h"

/**
 * @brief Abstract class representing an enemy.
 *
 * This class stores the attack and defense properties of the enemy.
 * The subclasses have to set these properties and create the enemy's sprites.
 * Every enemy sprite must have at least the following animations
 * and with the four main directions:
 * - "walking": used when the enemy is in its normal state
 * - "hurt": used when it was just hurt
 * If the enemy can be immobilized (i.e. it is sensible to attacks like the boomerang),
 * the following animations are also required:
 * - "immobilized": used while the enemy is immobilized
 * - "shaking": used when the immobilized enemy is about to get back to its normal state.
 * The animation of its sprites
 * automically switches between those animations 
 * depending on its current movement and the attacks it is subject to.
 * Additional animations may be defined by the subclasses.
 */
class ScriptedEntity: public Detector {

  public:

    // friend class ScriptedEntityScript;     // allow enemy scripts to access private data
    friend class Script;          // allow scripts with the enemy API to access private data

    EntityScript* script;           /**< the script that controls this enemy */

  protected:

    /**
     * @brief Defines how an enemy behaves with obstacles.
     */
    enum ObstacleBehavior {
      OBSTACLE_BEHAVIOR_NORMAL,   /**< the enemy is on the ground and stops on normal obstacles */
      OBSTACLE_BEHAVIOR_FLYING,   /**< the enemy ignores holes, lava, water, etc. */
      OBSTACLE_BEHAVIOR_SWIMMING, /**< the enemy can move in water */
      OBSTACLE_BEHAVIOR_NUMBER
    };

    ObstacleBehavior obstacle_behavior;                 /**< behavior with obstacles */
    static const std::string obstacle_behavior_names[]; /**< name of each existing behavior with obstacles */

    bool displayed_in_y_order;                          /**< indicates that the enemy is displayed as the same level as the hero */

  protected:

    // creation
    ScriptedEntity(const std::string &name, const std::string &script_name);
    ScriptedEntity(int collision_mode, const std::string &name, Layer layer,
	int x, int y, int width, int height);

    // functions available to the subclasses to define the enemy type properties (they can also change directly the fields)
    ObstacleBehavior get_obstacle_behavior();
    void set_obstacle_behavior(ObstacleBehavior obstacle_behavior);

    // hurt the enemy
    bool is_sprite_finished_or_looping();

    // animation
    const std::string& get_animation();
    void set_animation(const std::string &animation);

  public:

    // creation and destruction
    virtual ~ScriptedEntity();

    EntityType get_type();
    virtual void set_map(Map &map);
    virtual void notify_map_started();
    virtual void notify_map_opening_transition_finished();

    // obstacles
    bool is_obstacle_for(MapEntity& other);
    bool is_destructible_item_obstacle(DestructibleItem& destructible_item);
    bool is_teletransporter_obstacle(Teletransporter& teletransporter);
    bool is_deep_water_obstacle();
    bool is_shallow_water_obstacle();
    bool is_hole_obstacle();
    bool is_prickle_obstacle();
    bool is_lava_obstacle();

    // enemy state
    bool is_displayed_in_y_order();
    virtual void update();
    virtual void set_suspended(bool suspended);
    void notify_enabled(bool enabled);
    void notify_collision(MapEntity &entity_overlapping, CollisionMode collision_mode);
    void notify_collision(MapEntity &other_entity, Sprite &other_sprite, Sprite &this_sprite);
    void notify_collision_with_explosion(Explosion &explosion, Sprite &sprite_overlapping);
    void notify_collision_with_fire(Fire& fire, Sprite& sprite_overlapping);

    // attack the hero
    void attack_hero(Hero &hero, Sprite *this_sprite);
    void attack_stopped_by_hero_shield();
    bool get_can_attack();
    void set_can_attack(bool can_attack);

    static const std::string& get_obstacle_behavior_name(ObstacleBehavior behavior);
    static ObstacleBehavior get_obstacle_behavior_by_name(const std::string& name);

    // communication with others
    virtual void notify_message_received(ScriptedEntity& sender, const std::string& message);
};

#endif

