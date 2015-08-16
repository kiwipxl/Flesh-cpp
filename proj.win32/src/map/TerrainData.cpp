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
TerrainDataGroup* load(std::string file_name) {
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

        TerrainDataPtr ter(new TerrainData());

        std::string data = temp;
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
        if (split_tokens(data, tokens, "edge_indices:", '-')) {
            if (tokens.size() >= 2) {
                ter->set_edge_index_attrib(std::stof(tokens[0]), std::stof(tokens[1]));
            }else {
                RETURN_LOAD_ERR("edge_indices attribute expects more than 2 tokens", file_name);
            }
		}else {
            RETURN_LOAD_ERR("edge_indices attribute missing", file_name);
        }
        if (split_tokens(data, tokens, "fill_indices:", '-')) {
            if (tokens.size() >= 2) {
				ter->set_fill_index_attrib(std::stof(tokens[0]), std::stof(tokens[1]));
            }else {
                RETURN_LOAD_ERR("fill_indices attribute expects more than 2 tokens", file_name);
            }
		}else {
            RETURN_LOAD_ERR("fill_indices attribute missing", file_name);
        }
		delete[] temp;
	}else {
        RETURN_LOAD_ERR("file could not be found", file_name);
    }

    fclose(f);

    return tgroup.get();
}

END_TERRAIN_NS
END_MAP_NS