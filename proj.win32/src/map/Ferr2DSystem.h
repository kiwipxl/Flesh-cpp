#ifndef _FERR_2D_SYSTEM_
#define _FERR_2D_SYSTEM_

#include <string>
#include <sstream>
#include <vector>
#include "../SceneManager.h"

#define FERR2D_LOAD_ERROR_UNKNOWN_FILE 20000
#define FERR2D_LOAD_ERROR_VERTEX_DATA_MISSING 20001
#define FERR2D_LOAD_ERROR_INDICES_MISSING 20002
#define FERR2D_LOAD_ERROR_UV_DATA_MISSING 20003
#define FERR2D_LOAD_ERROR_COLLIDER_POINTS_MISSING 20004
#define FERR2D_LOAD_ERROR_ATTRIB_EDGE_INDICES_MISSING 20005
#define FERR2D_LOAD_ERROR_ATTRIB_FILL_INDICES_MISSING 20006
#define FERR2D_LOAD_ERROR_FILE_LEN_LZERO 20007

namespace ferr2d {

	class Terrain {

		public:
            Terrain(TerrainData& t_data);

            cc::Node* node;
            cc::PhysicsBody* pbody;
            TerrainData* terrain_data;

        private:
            cc::TrianglesCommand edge_tris_cmd;
            cc::TrianglesCommand::Triangles edge_tris;
            cc::TrianglesCommand fill_tris_cmd;
            cc::TrianglesCommand::Triangles fill_tris;
    };

    class TerrainData {

		public:
            //geometry
			std::vector<cc::V3F_C4B_T2F> points;
			std::vector<cc::Vec2> collider_points;
            std::vector<u_short> indices;

            //debug
            cc::DrawNode* debug_draw_node;
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

	extern TerrainData* load(std::string file_name);
	extern void print_load_error(int err);

};

#endif