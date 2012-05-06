import sys
import collections

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

        # use a dict that has an empty list as default value
        entities = collections.defaultdict(lambda: [])
        # load all remaining lines, ie, entities, and sort them by entity type
        for line in f:
            pieces = line.split()
            entities[int(pieces[0])].append(pieces[1:])

        entity_types = {
                 0: [ 'tiles'              , '{}, {}, {}, {}, {}, {}' ],
                 1: [ 'destinations'       , '{}, {}, {}, "{:s}", {}' ],
                 2: [ 'teletransporters'   , '{}, {}, {}, {}, {}, "{:s}", {}, "{:s}"' ],
                 3: [ 'pickable_items'     , '{}, {}, {}, "{:s}", {}, {}' ],
                 4: [ 'destructible_items' , '{}, {}, {}, {}, "{:s}", {}' ],
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

        for i in entity_types:
            # don't bother if there are no items
            if i not in entities:
                continue

            print '\
\
            ["{}"] = {{'.format(entity_types[i][0])

            for entity in entities[i]:
                # note those fields were split by whitespace before, that means strings 
                # are in quotes already, we don't need to take care of that!
                print '\
                {', entity_types[i][1].format(*entity), '},'

            print "\
            },"

        print "\
        }\
    }"
