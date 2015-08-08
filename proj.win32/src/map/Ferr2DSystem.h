#ifndef _FERR_2D_SYSTEM_
#define _FERR_2D_SYSTEM_

#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>

#include <renderer/CCTrianglesCommand.h>
#include <2d/CCDrawNode.h>

#define FERR2D_LOAD_ERROR_UNKNOWN_FILE 20000
#define FERR2D_LOAD_ERROR_VERTEX_DATA_MISSING 20001
#define FERR2D_LOAD_ERROR_INDICES_MISSING 20002
#define FERR2D_LOAD_ERROR_UV_DATA_MISSING 20003
#define FERR2D_LOAD_ERROR_COLLIDER_POINTS_MISSING 20004
#define FERR2D_LOAD_ERROR_ATTRIB_EDGE_INDICES_MISSING 20005
#define FERR2D_LOAD_ERROR_ATTRIB_FILL_INDICES_MISSING 20006
#define FERR2D_LOAD_ERROR_FILE_LEN_LZERO 20007

namespace ferr2d {

    using namespace cocos2d;

    class TerrainData {

		public:
            //geometry
			std::vector<V3F_C4B_T2F> points;
			std::vector<Vec2> collider_points;
            std::vector<u_short> indices;

            //debug
            DrawNode* debug_draw_node;
			std::vector<Vec2> debug_points;

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

            Node* node;
            PhysicsBody* pbody;
            TerrainData* terrain_data;

            void draw();

        private:
            TrianglesCommand edge_tris_cmd;
            TrianglesCommand::Triangles edge_tris;
            TrianglesCommand fill_tris_cmd;
            TrianglesCommand::Triangles fill_tris;
    };

	extern TerrainData* load(std::string file_name);
	extern void print_load_error(int err);

};

#endif