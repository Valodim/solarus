-- Generic script for an enemy that goes towards the
-- the hero if he sees him, and walks randomly otherwise.
-- The enemy has only one sprite. See generic_soldier.lua
-- for an enemy with a sword.

-- Example of use from an enemy script:

-- sol.main.include("enemies/generic_towards_hero")
-- set_properties({
--   sprite = "enemies/globul",
--   life = 4,
--   damage = 2,
--   normal_speed = 32,
--   faster_speed = 48,
--   hurt_style = "normal",
--   push_hero_on_sword = false,
--   pushed_when_hurt = true,
--   movement_create = function()
--     local m = sol.movement.create("random_path")
--     return m
--   end
-- })

-- The parameter of set_properties() is a table.
-- Its values are all optional except the sprite.

local properties = {}
local going_hero = false
local timer

function set_properties(prop)

  properties = prop
  -- set default values
  if properties.life == nil then
    properties.life = 2
  end
  if properties.damage == nil then
    properties.damage = 2
  end
  if properties.normal_speed == nil then
    properties.normal_speed = 32
  end
  if properties.faster_speed == nil then
    properties.faster_speed = 48
  end
  if properties.hurt_style == nil then
    properties.hurt_style = "normal"
  end
  if properties.pushed_when_hurt == nil then
    properties.pushed_when_hurt = true
  end
  if properties.push_hero_on_sword == nil then
    properties.push_hero_on_sword = false
  end
  if properties.movement_create == nil then 
    properties.movement_create = function()
      local m = sol.movement.create("random_path")
      return m
    end
  end
end

function event_appear()

  sol.enemy.set_life(properties.life)
  sol.enemy.set_damage(properties.damage)
  sol.enemy.create_sprite(properties.sprite)
  sol.enemy.set_hurt_style(properties.hurt_style)
  sol.enemy.set_pushed_back_when_hurt(properties.pushed_when_hurt)
  sol.enemy.set_push_hero_on_sword(properties.push_hero_on_sword)
  sol.enemy.set_size(16, 16)
  sol.enemy.set_origin(8, 13)
end

function event_movement_changed()

  local m = sol.enemy.get_movement()
  local direction4 = m:get_direction4()
  local sprite = sol.enemy.get_sprite()
  sprite:set_direction(direction4)
end

function event_obstacle_reached(movement)

  if not going_hero then
    go_random()
    check_hero()
  end
end

function event_restart()
  go_random()
  check_hero()
end

function event_hurt()
  if timer ~= nil then
    timer:stop()
    timer = nil
  end
end

function check_hero()

  local _, _, layer = sol.enemy.get_position()
  local _, _, hero_layer = sol.map.hero_get_position()
  local near_hero = layer == hero_layer
    and sol.enemy.get_distance_to_hero() < 100

  if near_hero and not going_hero then
    go_hero()
  elseif not near_hero and going_hero then
    go_random()
  end
  timer = sol.timer.start(1000, check_hero)
end

function go_random()
  local m = properties.movement_create()
  m:set_speed(properties.normal_speed)
  sol.enemy.start_movement(m)
  going_hero = false
end

function go_hero()
  local m = sol.movement.create("target")
  m:set_speed(properties.faster_speed)
  sol.enemy.start_movement(m)
  going_hero = true
end

