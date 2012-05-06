import sys

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print >> sys.stderr, "Usage: python", sys.argv[0], "mapXXXX.dat"

    fname = sys.argv[1]
    try:
        map_id = int(fname[3:7])
    except:
        print >> sys.stderr, "Invalid map filename, could not get mapid (need mapXXXX.dat)"
        sys.exit(1)

    with open(fname, "r") as f:
        first = f.readline()

        # load first line's parameters
        width, height, world, floor, x, y, small_keys_variable, tileset_id, music_id = first.split()

        print """
    return {{
        map_id = {:d},
        width = {:s},
        height = {:s},
        world = {:s},
        floor = {:s},
        x = {:s},
        y = {:s},
        small_keys_variable = {:s},
        tileset_id = "{:s}",
        music_id = "{:s}",
        entities = {{""".format(map_id, width, height, world, floor, x, y, small_keys_variable, tileset_id, music_id)

        # I used to do something with those names, but we don't really need them anymore. Ah well.
        entity_types = {
                 0: [ 'tiles'              , '{}, {}, {}, {}, {}, {}' ],
                 1: [ 'destinations'       , '{}, {}, {}, "{:s}", {}, {}' ],
                 2: [ 'teletransporters'   , '{}, {}, {}, {}, {}, "{:s}", {}, "{:s}"' ],
                 3: [ 'pickable_items'     , '{}, {}, {}, "{:s}", {}, {}' ],
                 4: [ 'destructible_items' , '{}, {}, {}, {}, "{:s}", {}, {}' ],
                 5: [ 'chests'             , '{}, {}, {}, "{:s}", {}, "{:s}", {}, {}' ],
                 6: [ 'jump_sensors'       , '{}, {}, {}, {}, {}, "{:s}", {}, {}' ],
                 7: [ 'enemys'             , '{}, {}, {}, "{:s}", {}, "{:s}", {}, {}, "{:s}", {}, {}' ],
                 8: [ 'NPCs'               , '{}, {}, {}, "{:s}", {}, {}, "{:s}", "{:s}"' ],
                 9: [ 'blocks'             , '{}, {}, {}, "{:s}", {}, "{:s}", {}' ],
                10: [ 'dynamic_tiles'      , '{}, {}, {}, {}, {}, "{:s}", {}, {}' ],
                11: [ 'switches'           , '{}, {}, {}, "{:s}", {}, {}' ],
                12: [ 'custom_obstacles'   , '{}, {}, {}, {}, {}, "{:s}", {}, {}, {}, {}' ],
                13: [ 'sensors'            , '{}, {}, {}, {}, {}, "{:s}", {}' ],
                14: [ 'crystals'           , '{}, {}, {}' ],
                15: [ 'crystal_blocks'     , '{}, {}, {}, {}, {}, {}' ],
                16: [ 'shop items'         , '{}, {}, {}, "{:s}", "{:s}", {}, {}, {}, "{:s}"' ],
                17: [ 'conveyor_belts'     , '{}, {}, {}, {}, {}, {}' ],
                18: [ 'doors'              , '{}, {}, {}, "{:s}", {}, {}' ],
                19: [ 'stairs'             , '{}, {}, {}, "{:s}", {}, {}' ],
        }


        # load all remaining lines, ie, entities, and just pass them through in lua format
        for line in f:
            pieces = line.split()
            print '\
            {', pieces[0], ',', entity_types[int(pieces[0])][1].format(*pieces[1:]), '},'

        print "\
        }\n\
    }"
