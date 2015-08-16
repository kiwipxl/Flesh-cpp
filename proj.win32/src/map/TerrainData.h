#ifndef _TERRAIN_DATA_H_
#define _TERRAIN_DATA_H_

#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>

#include <renderer/CCTrianglesCommand.h>
#include <2d/CCDrawNode.h>

#include "map/MapDefines.h"

BEGIN_MAP_NS
BEGIN_TERRAIN_NS

namespace cc = cocos2d;

class TerrainDataGroup;

class TerrainData {

public:
    //base
    TerrainDataGroup* parent;

    //geometry
    std::vector<cc::V3F_C4B_T2F> points;
    std::vector<cc::Vec2> collider_points;
    std::vector<u_short> indices;

    //debug
    std::vector<cc::Vec2> debug_points;

    //attribs
    std::string name;
    cc::Vec2 pos;
    int edge_indices_start;
    int edge_indices_end;
    int fill_indices_start;
    int fill_indices_end;
    cc::Vec2 min{ INT_MAX, INT_MAX };
    cc::Vec2 max{ -INT_MAX, -INT_MAX };

    void set_edge_index_attrib(int start, int end) {
        edge_indices_start = start;
        edge_indices_end = end;
    }

    void set_fill_index_attrib(int start, int end) {
        fill_indices_start = start;
        fill_indices_end = end;
    }
};

typedef std::shared_ptr<TerrainData> TerrainDataPtr;

class TerrainDataGroup {

public:
    std::vector<TerrainDataPtr> data_vec;
    std::vector<cc::Vec2> spawn_points;
    cc::Vec2 min;
    cc::Vec2 max;
    int max_width;
    int max_height;
};

typedef std::shared_ptr<TerrainDataGroup> TerrainDataGroupPtr;

extern TerrainDataGroupPtr load(std::string file_name);

END_TERRAIN_NS
END_MAP_NS

#endif