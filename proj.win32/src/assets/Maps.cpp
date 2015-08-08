#include "Maps.h"

#include "debug/Logger.h"

namespace assets {

    namespace maps {
        
        map::ferr2d::TerrainData* test_terrain;

        void init() {
            test_terrain = map::ferr2d::load("terrain.t2d");

            log_print_file << "map assets loaded";
        }
    };
};