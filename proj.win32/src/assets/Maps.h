#ifndef _MAPS_H_
#define _MAPS_H_

#include "map/Terrain.h"
#include "assets/AssetsDefines.h"

BEGIN_ASSETS_NS

namespace maps {

    extern map::terrain::TerrainDataGroup* test_terrain;

    extern void init();
};

END_ASSETS_NS

#endif