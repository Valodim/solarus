-- Mini Mowatulla: a small spider that comes from an egg.
-- This enemy is usually be generated by a bigger one.

in_shell = nil

-- The enemy appears: create its movement
function event_appear()
  
  sol.enemy.set_life(2)
  sol.enemy.set_damage(2)
  sol.enemy.create_sprite("enemies/mini_mowatulla")
  sol.enemy.set_size(16, 16)
  sol.enemy.set_origin(8, 13)
  sol.enemy.set_invincible()

  local sprite = sol.enemy.get_sprite()
  sprite:set_animation("shell")
  in_shell = true
  sol.timer.start(1000, break_shell)
end

-- The enemy was stopped for some reason and should restart
function event_restart()

  if in_shell then
    local sprite = sol.enemy.get_sprite()
    sprite:set_animation("shell")
    local m = sol.movement.create("target")
    m:set_speed(64)
    sol.enemy.start_movement(m)
  else
    local m = sol.movement.create("path_finding")
    m:set_speed(64)
    sol.enemy.start_movement(m)
  end
end

-- Starts breaking the shell
function break_shell()

  local sprite = sol.enemy.get_sprite()
  sol.enemy.stop_movement()
  sprite:set_animation("shell_breaking")
end

--  The animation of a sprite is finished
function event_sprite_animation_finished(sprite, animation)

  -- if the shell was breaking, let the mini mowatulla go
  if animation == "shell_breaking" then
    sprite:set_animation("walking")
    sol.enemy.snap_to_grid()
    sol.enemy.set_default_attack_consequences()
    in_shell = false
    sol.enemy.restart()
  end
end

