package.path = './quests/zsdx/data/?.lua'

Timer = require 'classes/Timer'

-- just a collection of static methods
Main = { 
    -- ticker, executed every tick
    tickers = { },
    -- total time passed in this gmae so far
    gametime = 0
}

-- special function: called every tick by the engine!
function sol.events.update()
    for k, t in ipairs(Main.tickers) do
        t:update()
    end

    -- end of the tick - update gametime
    Main.gametime = Main.gametime + 1
end

-- create a new timer for testing
x = Timer:new(2000, function () 
        print("this should never happen!")
    end)
-- and destroy it right away!
x:destroy()

-- and another one that actually runs
x = Timer:new(2000, function () 
        print("two thousand ticks passed!")
    end)
