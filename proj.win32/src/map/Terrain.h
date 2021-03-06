#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>

#include <renderer/CCTrianglesCommand.h>
#include <2d/CCDrawNode.h>

#include "map/MapDefines.h"
#include "map/TerrainData.h"

BEGIN_MAP_NS
BEGIN_TERRAIN_NS

namespace cc = cocos2d;

class TerrainGroup;

class Terrain {

	public:
        Terrain(TerrainData& t_data, TerrainGroup& t_group);

        cc::Node* base;
        cc::PhysicsBody* pbody;
        TerrainData* terrain_data;
        cc::DrawNode* debug_draw_node = NULL;

        void show_debug_geometry(bool show_triangles = true, bool show_collider_points = true);
        void hide_debug_geometry();
        void toggle_debug_geometry();
        void draw();

        void test_explosion_at(cc::Vec2 vec);

        bool is_debug_draw_on() { return debug_draw_on; }
        bool has_collider_points() { return has_collider; }

    private:
        cc::TrianglesCommand edge_tris_cmd;
        cc::TrianglesCommand::Triangles edge_tris;
        cc::TrianglesCommand fill_tris_cmd;
        cc::TrianglesCommand::Triangles fill_tris;

        bool debug_draw_on = false;
        bool has_collider = false;

        void create_debug_geometry(bool show_triangles = true, bool show_collider_points = true);
        void remove_debug_geometry();
};

class TerrainGroup {

public:
    TerrainGroup(TerrainDataGroup* data_group);

    cc::Node* base;
    std::vector<Terrain*> terrain_list;

    void show_debug_geometry(bool show_triangles = true, bool show_collider_points = true);
    void hide_debug_geometry();
    void toggle_debug_geometry();
    void draw();
    Terrain* is_terrain(cc::Node* a, cc::Node* b);

    bool is_debug_draw_on() { return debug_draw_on; }

private:
    bool debug_draw_on = false;
};

END_TERRAIN_NS
END_MAP_NS

#endif