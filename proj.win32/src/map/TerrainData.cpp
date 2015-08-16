#include "map/TerrainData.h"

#include "debug/Logger.h"
#include "StateManager.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

BEGIN_MAP_NS
BEGIN_TERRAIN_NS

using namespace cocos2d;

//private
void print_load_error(std::string err_message, std::string file_name) {
    f_assert(sstream_cstr("ferr2d load error occurred: " << err_message << " (" << file_name << ")"));
}

bool split_tokens(std::string& data, std::vector<std::string>& tokens, std::string attrib_name, char delimiter = ',') {
    int index = -1;
    int nl_index = -1;
    int co_index = -1;
    if ((index = data.find(attrib_name)) != -1 &&
        (nl_index = data.substr(index).find('\n')) != -1 &&
        (co_index = (data.substr(index).find(':') + 1)) != -1) {
        tokens = split(data.substr(index + co_index, nl_index - co_index), delimiter);
        return true;
    }
    tokens.clear();
    return false;
}

//handles load errors when loading terrain, prob should be inline function but gud nuf
#define RETURN_LOAD_ERR(err_message, file_name)                 \
    if (temp != NULL) delete[] temp;                            \
    print_load_error(err_message, file_name);                   \
    fclose(f);                                                  \
    return NULL;

//public
TerrainDataGroupPtr load(std::string file_name) {
    TerrainDataGroupPtr tgroup(new TerrainDataGroup());

    FILE* f = fopen(file_name.c_str(), "r");
    char* temp = NULL;

    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        int file_len = ftell(f);
        rewind(f);

        if (file_len <= 0) { print_load_error("file length cannot be less than 0", file_name); return NULL; }
        temp = new char[file_len];
        fread(temp, 1, file_len, f);

        std::string all_data = temp;
        int last_header_end = 0;
        const int header_begin_size = strlen("TERRAIN_BEGIN");
        const int header_end_size = strlen("TERRAIN_END");
        while (true) {
            auto l = all_data.length();
            int header_begin = all_data.substr(last_header_end).find("TERRAIN_BEGIN");
            int header_end = all_data.substr(last_header_end).find("TERRAIN_END");
            if (header_begin == -1 || header_end == -1) break;
            header_begin += last_header_end + header_begin_size;
            header_end += last_header_end + header_end_size;
            std::string data = all_data.substr(header_begin, header_end - header_begin);
            last_header_end = header_end;

            TerrainDataPtr ter(new TerrainData());
            tgroup->data_vec.push_back(ter);
            ter->parent = tgroup.get();

            std::vector<std::string> tokens;
		    if (split_tokens(data, tokens, "vertex_data:")) {
			    for (int n = 0; n < tokens.size(); n += 2) {
                    V3F_C4B_T2F v;
				    v.vertices.x = std::stof(tokens[n]) * 40.0f;
				    v.vertices.y = std::stof(tokens[n + 1]) * 40.0f;
				    v.colors = Color4B(255, 255, 255, 255);
				    ter->points.push_back(v);

				    Vec2 dv;
				    dv.x = v.vertices.x;
				    dv.y = v.vertices.y;
                    ter->debug_points.push_back(dv);

                    //calculate min/max x/y positions for the terrain
                    ter->min.x = MIN(v.vertices.x, ter->min.x);
                    ter->min.y = MIN(v.vertices.y, ter->min.y);

                    ter->max.x = MAX(v.vertices.x, ter->max.x);
                    ter->max.y = MAX(v.vertices.y, ter->max.y);
			    }
		    }else {
                RETURN_LOAD_ERR("vertex_data attribute missing", file_name);
            }
            if (split_tokens(data, tokens, "indices:")) {
                for (int n = 0; n < tokens.size(); ++n) {
                    ter->indices.push_back(std::stof(tokens[n]));
                }
		    }else {
                RETURN_LOAD_ERR("indices attribute missing", file_name);
            }
            if (split_tokens(data, tokens, "uvs:")) {
                for (int n = 0; n < tokens.size(); n += 2) {
				    V3F_C4B_T2F& v = ter->points[n / 2];
                    v.texCoords.u = std::stof(tokens[n]);
                    v.texCoords.v = 1 - std::stof(tokens[n + 1]);
                }
		    }else {
                RETURN_LOAD_ERR("uvs attribute missing", file_name);
            }
            if (split_tokens(data, tokens, "collider_points:")) {
                for (int n = 0; n < tokens.size(); n += 2) {
                    Vec2 v;
                    v.x = std::stof(tokens[n]) * 40.0f;
                    v.y = std::stof(tokens[n + 1]) * 40.0f;
				    ter->collider_points.push_back(v);
                }
            }
            if (split_tokens(data, tokens, "name:", ' ')) {
                if (tokens.size() >= 1) {
                    ter->name = tokens[0];
                }else {
                    RETURN_LOAD_ERR("name attribute expects more than 1 token", file_name);
                }
            }
            if (split_tokens(data, tokens, "pos:", ',')) {
                if (tokens.size() >= 2) {
                    ter->pos.x = std::stof(tokens[0]);
                    ter->pos.y = std::stof(tokens[1]);
                }else {
                    RETURN_LOAD_ERR("pos attribute expects more than 2 tokens", file_name);
                }
            }
            if (split_tokens(data, tokens, "edge_indices:", ',')) {
                if (tokens.size() >= 2) {
                    ter->set_edge_index_attrib(std::stof(tokens[0]), std::stof(tokens[1]));
                }else {
                    RETURN_LOAD_ERR("edge_indices attribute expects more than 2 tokens", file_name);
                }
		    }else {
                RETURN_LOAD_ERR("edge_indices attribute missing", file_name);
            }
            if (split_tokens(data, tokens, "fill_indices:", ',')) {
                if (tokens.size() >= 2) {
				    ter->set_fill_index_attrib(std::stof(tokens[0]), std::stof(tokens[1]));
                }else {
                    RETURN_LOAD_ERR("fill_indices attribute expects more than 2 tokens", file_name);
                }
		    }else {
                RETURN_LOAD_ERR("fill_indices attribute missing", file_name);
            }
        }
		delete[] temp;
	}else {
        RETURN_LOAD_ERR("file could not be found", file_name);
    }

    fclose(f);

    //calculate min, max, max_width and max_height of all terrain in tgroup
    for (int n = 0; n < tgroup->data_vec.size(); ++n) {
        auto t = tgroup->data_vec[n];

        tgroup->min.x = MIN(t->min.x, tgroup->min.x);
        tgroup->min.y = MIN(t->min.y, tgroup->min.y);

        tgroup->max.x = MAX(t->max.x, tgroup->max.x);
        tgroup->max.y = MAX(t->max.y, tgroup->max.y);

        tgroup->max_width = MAX(tgroup->max.x - tgroup->min.x, tgroup->max_width);
        tgroup->max_height = MAX(tgroup->max.y - tgroup->min.y, tgroup->max_height);
    }

    return tgroup;
}

END_TERRAIN_NS
END_MAP_NS