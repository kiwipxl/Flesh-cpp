#ifndef _FERR_2D_SYSTEM_
#define _FERR_2D_SYSTEM_

#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>

#include <renderer/CCTrianglesCommand.h>
#include <2d/CCDrawNode.h>

namespace cc = cocos2d;

namespace map {

    namespace ferr2d {

        class TerrainData {

		    public:
                //geometry
			    std::vector<cc::V3F_C4B_T2F> points;
			    std::vector<cc::Vec2> collider_points;
                std::vector<u_short> indices;

                //debug
			    std::vector<cc::Vec2> debug_points;

			    int edge_indices_start;
			    int edge_indices_end;
			    int fill_indices_start;
			    int fill_indices_end;

			    void set_edge_index_attrib(int start, int end) {
				    edge_indices_start = start;
				    edge_indices_end = end;
			    }

			    void set_fill_index_attrib(int start, int end) {
				    fill_indices_start = start;
				    fill_indices_end = end;
			    }
        };

	    class Terrain {

		    public:
                Terrain(TerrainData& t_data);

                cc::Node* base;
                cc::PhysicsBody* pbody;
                TerrainData* terrain_data;
                cc::DrawNode* debug_draw_node = NULL;

                void show_debug_geometry(bool show_triangles = true, bool show_collider_points = true);
                void hide_debug_geometry();
                void toggle_debug_geometry();
                void draw();

                bool is_debug_draw_on() { return debug_draw_on; }

            private:
                cc::TrianglesCommand edge_tris_cmd;
                cc::TrianglesCommand::Triangles edge_tris;
                cc::TrianglesCommand fill_tris_cmd;
                cc::TrianglesCommand::Triangles fill_tris;

                bool debug_draw_on = false;

                void create_debug_geometry(bool show_triangles = true, bool show_collider_points = true);
                void remove_debug_geometry();
        };

	    extern TerrainData* load(std::string file_name);
        extern void print_load_error(std::string err_message, std::string file_name);
    };
};

#endif