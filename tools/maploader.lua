
local entity_types = {}
for k, v in pairs({
                 [0] = 'Tile',
                 [1] = 'DestinationPoint',
                 [2] = 'Teletransporter',
                 [3] = 'PickableItem',
                 [4] = 'DestructibleItem',
                 [5] = 'Chest',
                 [6] = 'Jumper',
                 [7] = 'Enemy',
                 [8] = 'NPC',
                 [9] = 'Block',
                [10] = 'DynamicTile',
                [11] = 'Switch',
                [12] = 'CustomObstacle',
                [13] = 'Sensor',
                [14] = 'Crystal',
                [15] = 'CrystalBlock',
                [16] = 'ShopItem',
                [17] = 'ConveyorBelt',
                [18] = 'Door',
                [19] = 'Stairs',
        }) do
    entity_types[k] = luajava.bindClass("org.solarus.editor.entities." .. v)
end
local Layer = luajava.bindClass("org.solarus.editor.entities.Layer")

function loadMap(mapId, map)
    print("Loading " .. mapId .. " :)")

    map_data = require("map_" .. mapId .. "_data")

    -- Set a bunch of trivial data in this map
    map:setSize(map_data.width, map_data.height)
    map:setWorld(map_data.world)
    map:setFloor(map_data.floor)
    map:setLocation(map_data.x, map_data.y)
    map:setSmallKeysVariable(map_data.small_keys_variable)
    map:setTileset(map_data.tileset_id)
    map:setMusic(map_data.music_id)

    for k, e in ipairs(map_data.entities) do

        entity = luajava.new(entity_types[e[1]], map)

        entity:setLayer(Layer:get(e[2]))
        entity:setPositionTopLeftImpl(e[3], e[4])

        print(e[3], e[4])

        print('1')

        -- couple of properties
        pos = 5
        -- if the next two vars are sizes, ...
        if entity:isSizeVariable() then
            -- ... skip them until later
            pos = pos+2
        end

        print('2')
        if entity:hasName() then
            entity:setName(e[pos]);
            pos = pos+1
        end

        print('3')
        if entity:hasDirectionProperty() then
            entity:setDirection(e[pos])
            pos = pos+1
        end

        print('4')
        if entity:hasSubtype() then
            print(type(e[pos]))
            entity:setSubtypeId(e[pos])
            pos = pos+1
        end

        print('5')
        -- specific properties
        props = entity:getPropertyIterator()
        while props:hasNext() do
            name = props:next()
            entity:setProperty(name, e[pos])
            pos = pos+1
        end

        print('6')
        -- some entities need to know their properties before they can be resized
        if entity:isSizeVariable() then
            entity:setSize(e[5], e[6])
        end

        entity:updateImageDescription()

        entity:setPositionInMap(e[3], e[4])
        entity:setInitialized(true)

        map:addEntity(entity)
    end

    -- 0 = no errors. yay :)
    return 0

end
