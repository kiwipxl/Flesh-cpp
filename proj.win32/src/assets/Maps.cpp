#include "Maps.h"

#include "debug/Logger.h"

BEGIN_ASSETS_NS

namespace maps {
        
    map::terrain::TerrainDataGroup* test_terrain;

    void init() {
        test_terrain = map::terrain::load("terrain.f2d");

        log_print_file << "map assets loaded";
    }
};

END_ASSETS_NS
