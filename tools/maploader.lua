
-- Get bindings to entity types.
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
-- Also get a binding to Layer.
local Layer = luajava.bindClass("org.solarus.editor.entities.Layer")

-- This function is called from Map.load_lua() and does all the loading of
-- maps.
function loadMap(mapId, map)
    print("Loading " .. mapId .. " :)")

    -- run the map file to get the data
    local map_data = require("map_" .. mapId .. "_data")

    -- Set a bunch of trivial data in this map
    map:setSize(map_data.width, map_data.height)
    map:setWorld(map_data.world)
    map:setFloor(map_data.floor)
    map:setLocation(map_data.x, map_data.y)
    map:setSmallKeysVariable(map_data.small_keys_variable)
    map:setTileset(map_data.tileset_id)
    map:setMusic(map_data.music_id)

    -- iterate over entities
    for k, e in ipairs(map_data.entities) do

        local entity = luajava.new(entity_types[e[1]], map)

        -- at this point, joining the attributes and just using entity:parse
        -- might be a better option, but I wanted to see how this works out as
        -- lua code.

        -- NOT A FAN OF THIS METHOD. A constructor for each entity type that
        -- just takes the right number of parameters would probably be a lot
        -- faster and cleaner.

        entity:setLayer(Layer:get(e[2]))
        entity:setPositionTopLeftImpl(e[3], e[4])

        -- couple of properties
        local pos = 5
        -- if the next two vars are sizes, ...
        if entity:isSizeVariable() then
            -- ... skip them until later
            pos = pos+2
        end

        -- Parse a couple of static properties
        if entity:hasName() then
            entity:setName(e[pos]);
            pos = pos+1
        end

        if entity:hasDirectionProperty() then
            entity:setDirection(e[pos])
            pos = pos+1
        end

        if entity:hasSubtype() then
            entity:setSubtypeId(e[pos])
            pos = pos+1
        end

        -- And put the rest inspecificProperties
        props = entity:getPropertyIterator()
        while props:hasNext() do
            name = props:next()
            entity:setProperty(name, e[pos])
            pos = pos+1
        end

        -- Some entities need to know their properties before they can be
        -- resized. Do that now.
        if entity:isSizeVariable() then
            entity:setSize(e[5], e[6])
        end

        -- Some more setup.
        entity:updateImageDescription()

        entity:setPositionInMap(e[3], e[4])
        entity:setInitialized(true)

        map:addEntity(entity)
    end

    -- 0 = no errors. yay :)
    return 0

end
