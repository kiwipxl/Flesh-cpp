#include "map/Ferr2DSystem.h"

#include "StateManager.h"
#include "entities/Unit.h"
#include "debug/Logger.h"

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

namespace ferr2d {

    //-- begin terrain class --

    Terrain::Terrain(TerrainData& t_data) {
        terrain_data = &t_data;

        t_data.debug_draw_node->retain();

        node = Node::create();

        pbody = PhysicsBody::createEdgePolygon(&t_data.collider_points[0], t_data.collider_points.size());
        pbody->setGravityEnable(false);
        pbody->setDynamic(false);
        //pbody->setCategoryBitmask(0x02);
        //pbody->setCollisionBitmask(0x04);
        //pbody->setContactTestBitmask(0x04);

        node->setPhysicsBody(pbody);
        node->setPosition(0, 0);
        //node->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        //node->setScale(1.1f, 1.1f);
        state::scene->addChild(node, 1);

        edge_tris.indices = &t_data.indices[t_data.edge_indices_start];
        edge_tris.indexCount = t_data.edge_indices_end;
        edge_tris.verts = &t_data.points[0];
        edge_tris.vertCount = t_data.points.size();

        Texture2D* edge_t = Director::getInstance()->getTextureCache()->addImage("MossyEdges.png");
        edge_t->retain();
        edge_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

        BlendFunc blend_func;
        blend_func.src = GL_SRC_ALPHA;
        blend_func.dst = GL_ONE_MINUS_SRC_ALPHA;

        edge_tris_cmd.init(0.0f, edge_t->getName(), state::scene->getGLProgramState(), blend_func, edge_tris, node->getNodeToWorldTransform(), 1);

        fill_tris.indices = &t_data.indices[t_data.fill_indices_start];
        fill_tris.indexCount = t_data.fill_indices_end;
        fill_tris.verts = &t_data.points[0];
        fill_tris.vertCount = t_data.points.size();

        Texture2D* fill_t = Director::getInstance()->getTextureCache()->addImage("MossyFill.png");
        fill_t->retain();
        fill_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

        fill_tris_cmd.init(0.0f, fill_t->getName(), state::scene->getGLProgramState(), blend_func, fill_tris, node->getNodeToWorldTransform(), 1);
    }

    void Terrain::draw() {
        Director::getInstance()->getRenderer()->addCommand(&fill_tris_cmd);
        Director::getInstance()->getRenderer()->addCommand(&edge_tris_cmd);
        terrain_data->debug_draw_node->draw(Director::getInstance()->getRenderer(), node->getNodeToWorldTransform(), 0);
    }

    //-- end terrain class --

    void print_load_error(int err) {
        log_error << "ferr2d load error occurred: " << err;
    }

    TerrainData* load(std::string file_name) {
	    TerrainData* ter = new TerrainData();
	
	    FILE* f = fopen(file_name.c_str(), "r");

        if (f != NULL) {
            fseek(f, 0, SEEK_END);
            int file_len = ftell(f);
            rewind(f);

            if (file_len <= 0) { print_load_error(FERR2D_LOAD_ERROR_FILE_LEN_LZERO); return NULL; }
            char* temp = new char[file_len];
            fread(temp, 1, file_len, f);

            ter->debug_draw_node = DrawNode::create();

            std::string data = temp;
            int index = -1;
            int nl_index = -1;
            int co_index = -1;
		    if ((index = data.find("vertex_data:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
			    std::vector<std::string> v_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

			    for (int n = 0; n < v_strs.size(); n += 2) {
                    V3F_C4B_T2F v;
				    v.vertices.x = std::stof(v_strs[n]) * 40.0f;
				    v.vertices.y = std::stof(v_strs[n + 1]) * 40.0f;
				    v.colors = Color4B(255, 255, 255, 255);
				    ter->points.push_back(v);

				    Vec2 dv;
				    dv.x = v.vertices.x;
				    dv.y = v.vertices.y;
				    ter->debug_points.push_back(dv);
			    }
		    }else {
			    delete[] temp;
			    print_load_error(FERR2D_LOAD_ERROR_VERTEX_DATA_MISSING);
			    return NULL;
            }
            if ((index = data.find("indices:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
                std::vector<std::string> i_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

                for (int n = 0; n < i_strs.size(); ++n) {
                    ter->indices.push_back(std::stof(i_strs[n]));

                    if (n >= 1) {
                        if (n % 3 == 0) {
						    ter->debug_draw_node->drawLine(ter->debug_points[ter->indices[ter->indices.size() - 2]], ter->debug_points[ter->indices[ter->indices.size() - 4]], Color4F(1.0f, 1.0f, 1.0f, .4f));
                        }else {
						    ter->debug_draw_node->drawLine(ter->debug_points[ter->indices[ter->indices.size() - 2]], ter->debug_points[ter->indices[ter->indices.size() - 1]], Color4F(1.0f, 1.0f, 1.0f, .4f));
                        }
                    }
                }
		    }else {
			    delete[] temp;
			    print_load_error(FERR2D_LOAD_ERROR_INDICES_MISSING);
			    return NULL;
            }
            if ((index = data.find("uvs:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
                std::vector<std::string> uv_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

                for (int n = 0; n < uv_strs.size(); n += 2) {
				    V3F_C4B_T2F& v = ter->points[n / 2];
                    v.texCoords.u = std::stof(uv_strs[n]);
                    v.texCoords.v = 1 - std::stof(uv_strs[n + 1]);
                }
		    }else {
			    delete[] temp;
			    print_load_error(FERR2D_LOAD_ERROR_UV_DATA_MISSING);
			    return NULL;
            }
            if ((index = data.find("collider_points:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
                std::vector<std::string> c_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

                for (int n = 0; n < c_strs.size(); n += 2) {
                    Vec2 v;
                    v.x = std::stof(c_strs[n]) * 40.0f;
                    v.y = std::stof(c_strs[n + 1]) * 40.0f;
				    ter->collider_points.push_back(v);

                    if (n >= 1) {
					    ter->debug_draw_node->drawLine(ter->collider_points[ter->collider_points.size() - 2], ter->collider_points[ter->collider_points.size() - 1], Color4F(0.0f, 0.0f, 1.0f, .8f));
                    }
                }
		    }else {
			    delete[] temp;
			    print_load_error(FERR2D_LOAD_ERROR_COLLIDER_POINTS_MISSING);
			    return NULL;
            }
            if ((index = data.find("edge_indices:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
                std::vector<std::string> attribs = split(data.substr(index + co_index, nl_index - co_index), '-');
                if (attribs.size() >= 2) {
				    ter->set_edge_index_attrib(std::stof(attribs[0]), std::stof(attribs[1]));
                }
		    }else {
			    print_load_error(FERR2D_LOAD_ERROR_ATTRIB_EDGE_INDICES_MISSING);
			    return NULL;
            }
            if ((index = data.find("fill_indices:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
                std::vector<std::string> attribs = split(data.substr(index + co_index, nl_index - co_index), '-');
                if (attribs.size() >= 2) {
				    ter->set_fill_index_attrib(std::stof(attribs[0]), std::stof(attribs[1]));
                }
		    }else {
			    delete[] temp;
			    print_load_error(FERR2D_LOAD_ERROR_ATTRIB_FILL_INDICES_MISSING);
			    return NULL;
            }
		    delete[] temp;
	    }else {
		    print_load_error(FERR2D_LOAD_ERROR_UNKNOWN_FILE);
        }

        fclose(f);

        return ter;
    }
};