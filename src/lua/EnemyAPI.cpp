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
#include "lua/EnemyScript.h"
#include "entities/Enemy.h"
#include "entities/Hero.h"
#include "entities/MapEntities.h"
#include "lowlevel/Debug.h"
#include "lowlevel/StringConcat.h"
#include "lowlevel/Geometry.h"
#include "Game.h"
#include "Map.h"
#include <lua.hpp>

/**
 * @brief Returns the name of the enemy.
 *
 * - Return value (string): the name of the enemy
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_name(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushstring(l, enemy.get_name().c_str());

  return 1;
}

/**
 * @brief Returns the current number of life points of the enemy.
 *
 * - Return value (integer): the current life
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_life(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushinteger(l, enemy.get_life());

  return 1;
}

/**
 * @brief Sets the number of life points of the enemy.
 *
 * - Argument 1 (integer): the new level of life
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_life(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int life = luaL_checkinteger(l, 1);
  enemy.set_life(life);

  return 0;
}

/**
 * @brief Adds some life points of the enemy.
 *
 * - Argument 1 (integer): the number of life points to add
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_add_life(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int points = luaL_checkinteger(l, 1);
  enemy.set_life(enemy.get_life() + points);

  return 0;
}

/**
 * @brief Sets the number of life points of the enemy.
 *
 * - Argument 1 (integer): the new level of life
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_remove_life(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int points = luaL_checkinteger(l, 1);
  enemy.set_life(enemy.get_life() - points);

  return 0;
}

/**
 * @brief Returns the number of life points that the enemy
 * removes from the hero when touching him
 * (when the hero's defense level is minimal)
 *
 * - Return value (integer): the damage that the hero can receive from this enemy
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_damage(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushinteger(l, enemy.damage_on_hero);

  return 1;
}

/**
 * @brief Sets the number of life points that the enemy
 * removes from the hero when touching him
 * (when the hero's defense level is minimal).
 *
 * - Argument 1 (integer): the new damage level to set
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_damage(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int damage = luaL_checkinteger(l, 1);
  enemy.damage_on_hero = damage;

  return 0;
}

/**
 * @brief Returns the number of magic points that the enemy
 * removes from the hero when touching him
 * (when the hero's defense level is minimal)
 *
 * - Return value (integer): the magic damage that the hero can receive from this enemy
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_magic_damage(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushinteger(l, enemy.magic_damage_on_hero);

  return 1;
}

/**
 * @brief Sets the number of magic points that the enemy
 * removes from the hero when touching him
 * (when the hero's defense level is minimal).
 *
 * - Argument 1 (integer): the new magic damage level to set
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_magic_damage(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int magic_damage = luaL_checkinteger(l, 1);
  enemy.magic_damage_on_hero = magic_damage;

  return 0;
}

/**
 * @brief Returns whether the enemy is pushed away when it is hurt.
 *
 * - Return value (boolean): true if the enemy is pushed away when it is hurt
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_is_pushed_back_when_hurt(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushboolean(l, enemy.pushed_back_when_hurt);

  return 1;
}

/**
 * @brief Sets whether the enemy is pushed away when it is hurt.
 *
 * - Argument 1 (boolean): true if the enemy is pushed away when it is hurt
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_pushed_back_when_hurt(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  bool push_back = lua_toboolean(l, 1);
  enemy.pushed_back_when_hurt = push_back;

  return 0;
}

/**
 * @brief Returns whether the hero is pushed away when he strikes the enemy with the sword.
 *
 * - Return value (boolean): true if the hero is pushed away when using the sword
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_push_hero_on_sword(lua_State* l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushboolean(l, enemy.get_push_hero_on_sword());

  return 1;
}

/**
 * @brief Sets whether the hero is pushed away when he strikes the enemy with the sword.
 *
 * - Argument 1 (boolean): true to push the hero away when using the sword
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_push_hero_on_sword(lua_State* l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  bool push = lua_toboolean(l, 1);
  enemy.set_push_hero_on_sword(push);

  return 0;
}

/**
 * @brief Returns whether the enemy can hurt the hero even when the hero is running.
 *
 * - Return value (boolean): true if the hero can be hurt by this enemy when running
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_can_hurt_hero_running(lua_State* l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushboolean(l, enemy.get_can_hurt_hero_running());

  return 1;
}

/**
 * @brief Sets whether the enemy can hurt the hero even when the hero is running.
 *
 * - Argument 1 (boolean): true to allow the enemy to attack the hero while he is running
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_can_hurt_hero_running(lua_State* l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  bool can_hurt_hero_running = lua_toboolean(l, 1);
  enemy.set_can_hurt_hero_running(can_hurt_hero_running);

  return 0;
}

/**
 * @brief Returns the style of sounds and animations to play when the enemy is
 * hurt or killed.
 *
 * - Return value (string): "normal", "monster" or "boss"
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_hurt_style(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  const std::string& style_name = Enemy::get_hurt_style_name(enemy.hurt_style);
  lua_pushstring(l, style_name.c_str());

  return 1;
}

/**
 * @brief Sets the style of sounds and animations to play when the enemy is
 * hurt or killed.
 *
 * - Argument 1 (string): "normal", "monster" or "boss"
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_hurt_style(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  const std::string& style_name = luaL_checkstring(l, 1);
  enemy.hurt_style = Enemy::get_hurt_style_by_name(style_name);

  return 0;
}

/**
 * @brief Returns whether the enemy can currently attack the hero.
 *
 * - Return value (boolean): true if the enemy can currently attack the hero
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_can_attack(lua_State* l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushboolean(l, enemy.get_can_attack());

  return 1;
}

/**
 * @brief Sets whether the enemy can currently attack the hero.
 *
 * - Argument 1 (boolean): true to allow the enemy to attack the hero
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_can_attack(lua_State* l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  bool can_attack = lua_toboolean(l, 1);
  enemy.set_can_attack(can_attack);

  return 0;
}

/**
 * @brief Returns the minimum level of shield that allows the hero
 * to stop attacks from this enemy
 *
 * - Return value (integer): the minimum level of shield
 * (0 means that the hero cannot stop the attacks)
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_minimum_shield_needed(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  int shield_level = enemy.minimum_shield_needed;
  lua_pushinteger(l, shield_level);

  return 1;
}

/**
 * @brief Sets the minimum level of shield that allows the hero
 * to stop attacks from this enemy
 *
 * - Argument 1 (integer): the minimum level of shield to set
 * (0 means that the hero cannot stop the attacks)
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_minimum_shield_needed(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int shield_level = luaL_checkinteger(l, 1);
  enemy.minimum_shield_needed = shield_level;

  return 0;
}

/**
 * @brief Sets how the enemy reacts when it receives the specified attack.
 *
 * - Argument 1 (string): name of the attack to set
 * ("sword", "thrown_item", "explosion", "arrow", "hookshot", "boomerang", or "fire")
 * - Argument 2 (integer or string): an integer means a number of life points lost by the enemy
 * (must be greater than 0),
 * a string can specify "ignored", "protected", "immobilize" or "custom"
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_attack_consequence(lua_State *l) {

  Script& script = get_script(l, 2);
  Enemy& enemy = script.get_enemy();

  const std::string& attack_name = luaL_checkstring(l, 1);
  EnemyAttack attack = Enemy::get_attack_by_name(attack_name);

  if (lua_isnumber(l, 2)) {
    int life_points = luaL_checkinteger(l, 2);
    Debug::check_assertion(life_points > 0, StringConcat() << "Invalid attack consequence: " << life_points);
    enemy.set_attack_consequence(attack, EnemyReaction::HURT, life_points);
  }
  else {
    // TODO: simplify or encapsulate the C++ part of specifying attack consequences
    // (but the important thing is that the Lua API is easy to use)
    const std::string& reaction_name = lua_tostring(l, 2);
    EnemyReaction::ReactionType reaction = EnemyReaction::get_reaction_by_name(reaction_name);
    enemy.set_attack_consequence(attack, reaction);
  }

  return 0;
}

/**
 * @brief Sets how the enemy reacts when one of its sprites
 * receives the specified attack.
 *
 * - Argument 1 (sprite): the sprite to consider
 * - Argument 2 (string): name of the attack to set
 * ("sword", "thrown_item", "explosion", "arrow", "hookshot", "boomerang", or "fire")
 * - Argument 3 (integer or string): an integer means a number of life points lost by the enemy
 * (must be greater than 0),
 * a string can specify "ignored", "protected", "immobilized" or "custom"
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_attack_consequence_sprite(lua_State *l) {

  Script& script = get_script(l, 3);
  Enemy& enemy = script.get_enemy();

  int sprite_handle = luaL_checkinteger(l, 1);
  const std::string& attack_name = luaL_checkstring(l, 2);
  Sprite& sprite = script.get_sprite(sprite_handle);
  EnemyAttack attack = Enemy::get_attack_by_name(attack_name);

  if (lua_isnumber(l, 3)) {
    int life_points = luaL_checkinteger(l, 3);
    Debug::check_assertion(life_points > 0, StringConcat() << "Invalid attack consequence: " << life_points);
    enemy.set_attack_consequence_sprite(sprite, attack, EnemyReaction::HURT, life_points);
  }
  else {
    const std::string& reaction_name = lua_tostring(l, 3);
    EnemyReaction::ReactionType reaction = EnemyReaction::get_reaction_by_name(reaction_name);
    enemy.set_attack_consequence_sprite(sprite, attack, reaction);
  }

  return 0;
}

/**
 * @brief Sets the default reactions to attacks for the enemy.
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_default_attack_consequences(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  enemy.set_default_attack_consequences();

  return 0;
}

/**
 * @brief Sets the default reactions to attacks for a particular sprite of the enemy.
 *
 * - Parameter 1 (sprite): the sprite to consider
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_default_attack_consequences_sprite(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();
  int sprite_handle = luaL_checkinteger(l, 1);
  Sprite& sprite = script.get_sprite(sprite_handle);

  enemy.set_default_attack_consequences_sprite(sprite);

  return 0;
}

/**
 * @brief Sets the enemy invincible.
 *
 * This is equivalent to setting all the attack consequences as "ignored".
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_invincible(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  enemy.set_no_attack_consequences();

  return 0;
}

/**
 * @brief Sets a particular sprite of the enemy invincible.
 *
 * This is equivalent to setting all the attack consequences as "ignored"
 * for that sprite.
 * - Parameter 1 (sprite): the sprite to consider
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_invincible_sprite(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();
  int sprite_handle = luaL_checkinteger(l, 1);
  Sprite& sprite = script.get_sprite(sprite_handle);

  enemy.set_no_attack_consequences_sprite(sprite);

  return 0;
}

/**
 * @brief Sets the treasure dropped by the enemy.
 *
 * - Argument 1 (string): name of the item (possibly "_random" or "_none")
 * - Argument 2 (integer): variant of the item
 * - Argument 3 (integer): savegame variable of the treasure
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_treasure(lua_State *l) {

  Script& script = get_script(l, 3);
  Enemy& enemy = script.get_enemy();

  const std::string& item_name = luaL_checkstring(l, 1);
  int variant = luaL_checkinteger(l, 2);
  int savegame_variable = luaL_checkinteger(l, 3);

  Treasure treasure(script.get_game(), item_name, variant, savegame_variable);
  enemy.set_treasure(treasure);

  return 0;
}

/**
 * @brief Sets the treasure dropped by an enemy as empty.
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_no_treasure(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  Treasure treasure(script.get_game(), "_none", 1, -1);
  enemy.set_treasure(treasure);

  return 0;
}

/**
 * @brief Sets the treasure dropped by an enemy as random.
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_set_random_treasure(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  Treasure treasure(script.get_game(), "_random", 1, -1);
  enemy.set_treasure(treasure);

  return 0;
}

/**
 * @brief Restarts the enemy. This plays animation "walking"
 * on its sprites and triggers event_restart().
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_restart(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  enemy.restart();

  return 0;
}

/**
 * @brief Hurts the enemy
 *
 * - Argument 1 (integer): number of life points to remove
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_hurt(lua_State *l) {

  Script& script = get_script(l, 1);
  Enemy& enemy = script.get_enemy();

  int life_points = luaL_checkinteger(l, 1);

  if (enemy.is_in_normal_state() && !enemy.invulnerable) {
    Hero& hero = script.get_game().get_hero();
    enemy.life -= life_points;
    enemy.hurt(hero);
    enemy.notify_hurt(hero, ATTACK_SCRIPT, life_points);
  }

  return 0;
}

/**
 * @brief Creates another enemy on the map.
 *
 * The current enemy will be considered as the father of the enemy created.
 *
 * - Argument 1 (string): name of the enemy to create
 * - Argument 2 (string): breed of the enemy to create
 * - Argument 3 (integer): x position relative to the father
 * - Argument 4 (integer): y position relative to the father
 * - Optional argument 5 (integer): layer of the new enemy (default is the
 * same as the father)
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_create_son(lua_State *l) {

  Script& script = get_script(l, 4, 5);
  Enemy& enemy = script.get_enemy();

  const std::string& name = luaL_checkstring(l, 1);
  const std::string& breed = luaL_checkstring(l, 2);
  int x = luaL_checkinteger(l, 3);
  int y = luaL_checkinteger(l, 4);
  int layer;
  if (lua_gettop(l) >= 5) {
    layer = luaL_checkinteger(l, 5);
  }
  else {
    layer = enemy.get_layer();
  }

  x += enemy.get_x();
  y += enemy.get_y();

  MapEntities& entities = script.get_map().get_entities();
  Treasure treasure = Treasure(script.get_game(), "_random", 1, -1);
  Enemy* son = (Enemy*) Enemy::create(script.get_game(), breed, Enemy::RANK_NORMAL, -1,
      name, Layer(layer), x, y, 0, treasure);
  son->father_name = enemy.get_name();
  son->set_optimization_distance(enemy.get_optimization_distance());
  entities.add_entity(son);
  son->restart();

  return 0;
}


/**
 * @brief Returns the name of the father of the enemy, i.e.
 * the enemy who created it (if any).
 *
 * - Return value (string): the name of the father, or an empty string
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_get_father(lua_State *l) {

  Script& script = get_script(l, 0);
  Enemy& enemy = script.get_enemy();

  lua_pushstring(l, enemy.father_name.c_str());

  return 1;
}

/**
 * @brief Sends a message to another enemy.
 *
 * The other enemy will receive the event event_message_received().
 *
 * - Argument 1 (string): name of the destination enemy
 * - Argument 2 (string): the message to send
 *
 * @param l the Lua context that is calling this function
 */
int Script::enemy_api_send_message(lua_State *l) {

  Script& script = get_script(l, 2);
  Enemy& enemy = script.get_enemy();

  const std::string& dst_enemy_name = luaL_checkstring(l, 1);
  const std::string& message = luaL_checkstring(l, 2);

  MapEntities& entities = script.get_map().get_entities();
  Enemy* dst_enemy = (Enemy*) entities.find_entity(ENEMY, dst_enemy_name);
  dst_enemy->notify_message_received(enemy, message);

  return 1;
}
