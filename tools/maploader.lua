function loadMap(mapId, map)
    print("Loading " .. mapId .. " :)")

    map_data = require("map_" .. mapId .. "_data")

    -- Set a bunch of trivial data in this map
    map:test()
    map:setSize(map_data.width, map_data.height)
    map:setWorld(map_data.world)
    map:setFloor(map_data.floor)
    map:setLocation(map_data.x, map_data.y)
    map:setSmallKeysVariable(map_data.small_keys_variable)
    map:setTileset(map_data.tileset)
    map:setMusic(map_data.music_id)

    -- 0 = no errors. yay :)
    return 0

end
