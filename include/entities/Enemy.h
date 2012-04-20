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
#ifndef SOLARUS_ENEMY_H
#define SOLARUS_ENEMY_H

#include "Common.h"
#include "Treasure.h"
#include "entities/Detector.h"
#include "entities/EnemyAttack.h"
#include "entities/EnemyReaction.h"
#include "entities/Explosion.h"
#include "entities/ScriptedEntity.h"

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
class Enemy: public ScriptedEntity {

  public:

    friend class EnemyScript;     // allow enemy scripts to access private data
    friend class Script;          // allow scripts with the enemy API to access private data

    /**
     * @brief Enemy ranks.
     */
    enum Rank {
      RANK_NORMAL,
      RANK_MINIBOSS,
      RANK_BOSS
    };

  protected:

    /**
     * @brief Defines the style of sounds and animations played when an enemy
     * is hurt or killed.
     */
    enum HurtStyle {
      HURT_NORMAL,   /**< "enemy_hurt" (and if necessary "enemy_killed") is played */
      HURT_MONSTER,  /**< "monster_hurt" (and if necessary "enemy_killed") is played */
      HURT_BOSS,     /**< "boss_hurt" or "boss_killed" is played and explosions are created */
      HURT_NUMBER
    };

    /**
     * @brief This structure contains the parameters needed by the subclasses constructors.
     */
    struct ConstructionParameters {
      Game &game;				/**< the game */
      const Treasure &treasure;			/**< the treasure dropped by the enemy */
      std::string name;				/**< name of the instance of enemy */
      Layer layer;				/**< initial layer of the enemy */
      int x;					/**< initial x coordinate of the enemy */
      int y;					/**< initial y coordinate of the enemy */
    };

  private:

    // attack/defense features of this type of enemy
    int damage_on_hero;					/**< number of heart quarters the player loses when he gets hurt by this enemy;
							 * this number is divided depending on the hero's tunic number (default: 1) */
    int magic_damage_on_hero;				/**< number of magic points the player loses when he gets hurt
							 * by this enemy (default: 0) */
    int life;						/**< number of health points of the enemy (default: 1) */
    HurtStyle hurt_style;			/**< style of sounds and animations when this enemy gets hurt
							 * (default: HURT_NORMAL) */
    static const std::string hurt_style_names[];  /**< name of each hurt style */
    bool pushed_back_when_hurt;				/**< indicates whether the enemy is pushed back when it gets hurt by the hero
							 * (default: true) */
    bool push_hero_on_sword;		          	/**< indicates whether the hero is pushed back when he hurts the enemy with his
							 * sword (default: false) */
    bool can_hurt_hero_running;             /**< indicates that the enemy can attack the hero even when the hero is running */
    int minimum_shield_needed;				/**< shield number needed by the hero to avoid the attack of this enemy,
							 * or 0 to make the attack unavoidable (default: 0) */

    EnemyReaction attack_reactions[ATTACK_NUMBER];      /**< indicates how the enemy reacts to each attack
							 * (by default, it depends on the attacks) */
    static const std::string attack_names[];            /**< name of each type of attack an enemy can receive */


    // enemy characteristics
    Rank rank;						/**< is this enemy a normal enemy, a miniboss or a boss? */
    int savegame_variable;				/**< index of the boolean variable indicating whether this enemy is killed,
							 * or -1 if it is not saved */
    std::string father_name;                            /**< name of the enemy who created this enemy (or an empty string) */

    // enemy state
    bool enabled;					/**< indicates that the enemy is enabled */
    bool being_hurt;					/**< indicates that the enemy is being hurt */
    uint32_t stop_hurt_date;				/**< date when the enemy stops being hurt */
    bool invulnerable;					/**< indicates that the enemy cannot be hurt for now */
    uint32_t vulnerable_again_date;			/**< date when the enemy can be hurt again */
    bool can_attack;					/**< indicates that the enemy can currently attack the hero */
    uint32_t can_attack_again_date;			/**< date when the enemy can attack again (0 means never) */
    bool immobilized;					/**< indicates that the enemy is currently immobilized */
    uint32_t start_shaking_date;			/**< date when the enemy shakes */ 
    uint32_t end_shaking_date;				/**< date when the enemy stops shaking and walks again */ 

    // treasure
    Treasure treasure;					/**< pickable item that appears when this enemy gets killed */

    // boss or mini-boss
    bool exploding;					/**< indicates that the boss is dying and some explosions are triggered on him */
    int nb_explosions;					/**< number of explosions already played */
    uint32_t next_explosion_date;			/**< date of the next explosion */

  protected:

    // creation
    Enemy(const ConstructionParameters &params);

    /**
     * @brief Initializes the features, the sprites and the movement.
     */
    virtual void initialize() = 0;
    virtual void restart();

    // functions available to the subclasses to define the enemy type properties (they can also change directly the fields)
    void set_damage(int damage_on_hero);
    void set_damage(int damage_on_hero, int magic_damage_on_hero);
    void set_life(int life);
    int get_life();
    ObstacleBehavior get_obstacle_behavior();
    void set_obstacle_behavior(ObstacleBehavior obstacle_behavior);
    void set_pushed_back_when_hurt(bool pushed_back_when_hurt);
    void set_push_hero_on_sword(bool push_hero_on_sword);
    void set_can_hurt_hero_running(bool can_hurt_hero_running);
    void set_attack_consequence(EnemyAttack attack, EnemyReaction::ReactionType reaction, int life_lost = 0);
    void set_attack_consequence_sprite(Sprite& sprite, EnemyAttack attack,
        EnemyReaction::ReactionType reaction, int life_lost = 0);
    void set_no_attack_consequences();
    void set_no_attack_consequences_sprite(Sprite& sprite);
    void set_default_attack_consequences();
    void set_default_attack_consequences_sprite(Sprite& sprite);

    // hurt the enemy
    void play_hurt_sound();
    bool is_in_normal_state();
    bool is_being_hurt();
    bool is_immobilized();
    bool is_killed();
    bool is_dying_animation_finished();
    void hurt(MapEntity &source);
    bool is_sprite_finished_or_looping();
    void immobilize();
    void stop_immobilized();
    virtual void custom_attack(EnemyAttack attack, Sprite* this_sprite);
    virtual void notify_hurt(MapEntity &source, EnemyAttack attack, int life_points);
    virtual void notify_dead();
    virtual void notify_immobilized();

    // animation
    const std::string& get_animation();
    void set_animation(const std::string &animation);

  public:

    // creation and destruction
    virtual ~Enemy();

    static CreationFunction parse;
    static MapEntity* create(Game& game,
	const std::string& breed, Rank rank, int savegame_variable,
	const std::string& name, Layer layer, int x, int y, int direction,
	const Treasure& treasure);

    EntityType get_type();
    virtual void set_map(Map &map);
    virtual void notify_map_started();
    virtual void notify_map_opening_transition_finished();
    Rank get_rank();

    // enemy state
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

    // receive an attack
    bool get_push_hero_on_sword();
    bool get_can_hurt_hero_running();
    const EnemyReaction::Reaction& get_attack_consequence(EnemyAttack attack, Sprite *this_sprite);
    void try_hurt(EnemyAttack attack, MapEntity &source, Sprite *this_sprite);
    void kill();
    bool is_dying();
    void set_treasure(const Treasure& treasure);

    static const std::string& get_attack_name(EnemyAttack attack);
    static EnemyAttack get_attack_by_name(const std::string& attack_name);

    static const std::string& get_hurt_style_name(HurtStyle style);
    static HurtStyle get_hurt_style_by_name(const std::string& name);

    static const std::string& get_obstacle_behavior_name(ObstacleBehavior behavior);
    static ObstacleBehavior get_obstacle_behavior_by_name(const std::string& name);

    // communication with others
    virtual void notify_message_received(Enemy& sender, const std::string& message);
};

#endif

