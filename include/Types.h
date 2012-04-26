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

/**
 * @file Types.h
 * @brief Some declarations of types.
 *
 * This header is included by Common.h and every class header file should include Common.h.
 */

#ifndef SOLARUS_TYPES_H
#define SOLARUS_TYPES_H

#include <stdint.h>
#include <string>

// types of the id of each resource.

/**
 * @brief Type of the id of images.
 */
typedef std::string ImageId;

/**
 * @brief Type of the id of maps.
 */
typedef uint32_t MapId;

/**
 * @brief Type of the id of tilesets.
 */
typedef uint32_t TilesetId;

/**
 * @brief Type of the id of musics.
 */
typedef std::string MusicId;

/**
 * @brief Type of the id of sounds.
 */
typedef std::string SoundId;

/**
 * @brief Type of the id of sprite animation sets.
 */
typedef std::string SpriteAnimationSetId;

// declaration of all classes to avoid dependencies between the header files

// main classes
class Solarus;
class Screen;
class Configuration;
class KeysEffect;

// low level
class System;
class FileTools;
class VideoManager;
class Surface;
class TextSurface;
class Color;
class Sound;
class Music;
class SpcDecoder;
class ItDecoder;
class Random;
class Geometry;
class Rectangle;
class PixelBits;
class IniFile;
class InputEvent;
class Debug;
class StringConcat;

// menus
class LanguageScreen;
class TitleScreen;
class SelectionMenu;
class SelectionMenuPhase;
class SelectionMenuEraseFile;
class SelectionMenuConfirmErase;
class SelectionMenuChooseName;
class SelectionMenuChooseMode;
class SelectionMenuOptions;
class PauseMenu;
class PauseSubmenu;
class PauseSubmenuInventory;
class PauseSubmenuMap;
class PauseSubmenuQuestStatus;
class PauseSubmenuOptions;
class StringResource;

// game
class Game;
class GameControls;
class Savegame;
class Equipment;
class ItemProperties;
class InventoryItem;
class DebugKeys;
class Dungeon;
class Treasure;
class Map;
class MapLoader;
class Timer;
class GameoverSequence;
class Camera;
class Dialog;
class DialogResource;
class DialogBox;

// scripts
class Script;
class MapScript;
class ItemScript;
class EnemyScript;
class StateScript;

// HUD
class HUD;
class HudElement;
class ActionIcon;
class SwordIcon;
class PauseIcon;
class ItemIcon;
class Counter;
class HeartsView;
class RupeesCounter;
class MagicBar;
class SmallKeysCounter;
class FloorView;

// sprites
class Sprite;
class SpriteAnimationSet;
class SpriteAnimation;
class SpriteAnimationDirection;

// transition effects
class Transition;
class TransitionFade;
class TransitionImmediate;
class TransitionScrolling;

// map entities
class MapEntities;
class MapEntity;
class Hero;
class HeroSprites;
class Tile;
class DynamicTile;
class Detector;
class Teletransporter;
class DestinationPoint;
class PickableItem;
class CarriedItem;
class DestructibleItem;
class Chest;
class Jumper;
class Enemy;
class CustomEnemy;
class EnemyReaction;
class NPC;
class Block;
class Switch;
class CustomObstacle;
class Sensor;
class Crystal;
class CrystalBlock;
class ShopItem;
class Boomerang;
class Arrow;
class ConveyorBelt;
class Stairs;
class Door;
class Explosion;
class Bomb;
class Fire;
class Hookshot;
class ScriptedEntity;

// tile patterns
class Tileset;
class TilePattern;
class SimpleTilePattern;
class AnimatedTilePattern;
class SelfScrollingTilePattern;
class TimeScrollingTilePattern;
class ParallaxScrollingTilePattern;

// movements
class Movement;
class StraightMovement;
class PlayerMovement;
class PixelMovement;
class FallingOnFloorMovement;
class JumpMovement;
class PathMovement;
class RandomPathMovement;
class PathFindingMovement;
class PathFinding;
class RandomMovement;
class FollowMovement;
class TargetMovement;
class CircleMovement;

#endif

