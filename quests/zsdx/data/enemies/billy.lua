-- Billy

local going_hero = false
local timer

function event_appear()

  sol.enemy.set_life(30)
  sol.enemy.set_damage(16)
  sol.enemy.create_sprite("enemies/billy")
  sol.enemy.set_hurt_style("boss")
  sol.enemy.set_pushed_back_when_hurt(true)
  sol.enemy.set_push_hero_on_sword(true)
  sol.enemy.set_size(16, 16)
  sol.enemy.set_origin(8, 13)
  sol.enemy.set_invincible()
  sol.enemy.set_attack_consequence("sword", 1)
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
    sol.main.timer_stop_all()
  end
end

function check_hero()

  local near_hero = sol.enemy.get_distance_to_hero() < 100
  if near_hero and not going_hero then
    go_hero()
  elseif not near_hero and going_hero then
    go_random()
  end
  timer = sol.timer.start(1000, check_hero)
end

function go_random()
  local m = sol.movement.create("random_path")
  m:set_speed(48)
  sol.enemy.start_movement(m)
  going_hero = false
end

function go_hero()
  local m = sol.movement.create("target")
  m:set_speed(72)
  sol.enemy.start_movement(m)
  going_hero = true
end

