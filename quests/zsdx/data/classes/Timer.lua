-- module housekeeping
local P = { }
setmetatable(P, {__index = _G})
setfenv(1, P)

Timer = {
    at = 0,
    f = nil
}

function Timer:new(time, f)
    o = {
        at = Main.gametime + time,
        f = f
    }
    setmetatable(o, self)
    self.__index = self
    table.insert(Main.tickers, o)
    o.tid = #Main.tickers
    return o
end

function Timer:update()
    if Main.gametime > self.at then
        if self.f then
            r = self.f()
            self:destroy()
        end
    end
end

function Timer:destroy()
    table.remove(Main.tickers, self.tid)
end

return Timer
