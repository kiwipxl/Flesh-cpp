#include "StateManager.h"
#include "debug/Errors.h"
#include "network/sockets/SocketManager.h"
#include "input/SimpleInput.h"
#include "network/Peers.h"
#include <stdio.h>

using state::State;

SceneManager* state::scene;

State state::s = state::SERVER_CONNECT_SCREEN;
cc::LabelBMFont* state::info_label;
float state::time_since_startup = 0;
cc::TextFieldDelegate username_input;

cc::Sprite* state::player;
cc::DrawNode* d_node;
std::vector<cc::V3F_C4B_T2F> points;
std::vector<cc::Vec2> debug_points;
std::vector<cc::Vec2> collider_points;

cc::TrianglesCommand edge_tris_cmd;
cc::TrianglesCommand::Triangles edge_tris;
cc::TrianglesCommand fill_tris_cmd;
cc::TrianglesCommand::Triangles fill_tris;
std::vector<u_short> indices;
int edge_indices_start;
int edge_indices_end;
int fill_indices_start;
int fill_indices_end;

cc::Node* terrain_node;

#include <string>
#include <sstream>
#include <vector>

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

void create_state(State c_state) {
    using namespace state;

    s = c_state;
    switch (s) {
        case SERVER_CONNECT_SCREEN:
            info_label = cc::LabelBMFont::create("", "fonts/lucida.fnt");
            info_label->setString("connecting...");
            //scene->addChild(info_label, 1);

            scene->scheduleUpdate();

            sock::setup_tcp_sock();
            break;
        case LOGIN_REGISTER_SCREEN:
            break;
    }
}

void remove_state(State r_state) {
    using namespace state;

    s = r_state;
    switch (s) {
        case SERVER_CONNECT_SCREEN:
            scene->removeChild(info_label);
            break;
    }
}

void state::switch_state(State new_state) {
    remove_state(s);
    create_state(new_state);
}

void state::init(SceneManager* scene_ref) {
    scene = scene_ref;
    scene->setGLProgram(cc::ShaderCache::getInstance()->getGLProgram(cc::GLProgram::SHADER_NAME_POSITION_COLOR));

    sock::init();
    input::init();

    create_state(s);

    d_node = cc::DrawNode::create();
    terrain_node = cc::Node::create();
    terrain_node->setPosition(500, 100);

    FILE* f = fopen("terrain.t2d", "r");

    if (f != NULL) {
        fseek(f, 0, SEEK_END);
        int file_len = ftell(f);
        rewind(f);

        char* temp = new char[file_len];
        fread(temp, 1, file_len, f);

        std::string data = temp;
        int index = -1;
        int nl_index = -1;
        int co_index = -1;
        if ((index = data.find("vertex_data:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
            std::vector<std::string> v_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

            for (int n = 0; n < v_strs.size(); n += 2) {
                cc::V3F_C4B_T2F v;
                v.vertices.x = std::stof(v_strs[n]) * 40.0f;
                v.vertices.y = std::stof(v_strs[n + 1]) * 40.0f;
                v.colors = cc::Color4B(255, 255, 255, 255);
                points.push_back(v);

                cc::Vec2 dv;
                dv.x = v.vertices.x;
                dv.y = v.vertices.y;
                debug_points.push_back(dv);
            }
        }
        if ((index = data.find("indices:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
            std::vector<std::string> i_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

            for (int n = 0; n < i_strs.size(); ++n) {
                indices.push_back(std::stof(i_strs[n]));

                if (n >= 1) {
                    if (n % 3 == 0) {
                        d_node->drawLine(debug_points[indices[indices.size() - 2]], debug_points[indices[indices.size() - 4]], cc::Color4F(1.0f, 1.0f, 1.0f, .4f));
                    }else {
                        d_node->drawLine(debug_points[indices[indices.size() - 2]], debug_points[indices[indices.size() - 1]], cc::Color4F(1.0f, 1.0f, 1.0f, .4f));
                    }
                }
            }
        }
        if ((index = data.find("uvs:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
            std::vector<std::string> uv_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

            for (int n = 0; n < uv_strs.size(); n += 2) {
                cc::V3F_C4B_T2F& v = points[n / 2];
                v.texCoords.u = std::stof(uv_strs[n]);
                v.texCoords.v = 1 - std::stof(uv_strs[n + 1]);
            }
        }
        if ((index = data.find("collider_points:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
            std::vector<std::string> c_strs = split(data.substr(index + co_index, nl_index - co_index), ',');

            for (int n = 0; n < c_strs.size(); n += 2) {
                cc::Vec2 v;
                v.x = std::stof(c_strs[n]) * 40.0f;
                v.y = std::stof(c_strs[n + 1]) * 40.0f;
                collider_points.push_back(v);

                if (n >= 1) {
                    d_node->drawLine(collider_points[collider_points.size() - 2], collider_points[collider_points.size() - 1], cc::Color4F(0.0f, 0.0f, 1.0f, .8f));
                }
            }
        }
        if ((index = data.find("edge_indices:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
            std::vector<std::string> attribs = split(data.substr(index + co_index, nl_index - co_index), '-');
            if (attribs.size() >= 2) {
                edge_indices_start = std::stof(attribs[0]);
                edge_indices_end = std::stof(attribs[1]);
            }
        }
        if ((index = data.find("fill_indices:")) != -1 && (nl_index = data.substr(index).find('\n')) != -1 && (co_index = (data.substr(index).find(':') + 1)) != -1) {
            std::vector<std::string> attribs = split(data.substr(index + co_index, nl_index - co_index), '-');
            if (attribs.size() >= 2) {
                fill_indices_start = std::stof(attribs[0]);
                fill_indices_end = std::stof(attribs[1]);
            }
        }
    }

    fclose(f);

    player = cc::Sprite::create("HelloWorld.png");
    player->setPosition(player->getContentSize().width, player->getContentSize().height);
    player->setScale(.25f);
    player->retain();
    scene->addChild(player, 1);

    auto physics_body = cc::PhysicsBody::createBox(cc::Size(player->getContentSize().width * player->getScaleX(), 
                                                            player->getContentSize().height * player->getScaleY()));
    player->setPhysicsBody(physics_body);

    scene->p_world->setDebugDrawMask(cc::PhysicsWorld::DEBUGDRAW_ALL);

    auto dphysics_body = cc::PhysicsBody::createEdgePolygon(&collider_points[0], collider_points.size());
    dphysics_body->setGravityEnable(false);
    dphysics_body->setDynamic(false);
    terrain_node->setPhysicsBody(dphysics_body);
    scene->addChild(terrain_node, 1);
    d_node->retain();

    edge_tris.indices = &indices[edge_indices_start];
    edge_tris.indexCount = edge_indices_end;
    edge_tris.verts = &points[0];
    edge_tris.vertCount = points.size();

    cc::Texture2D* edge_t = cc::Director::getInstance()->getTextureCache()->addImage("MossyEdges.png");
    edge_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

    edge_tris_cmd.init(0.0f, edge_t->getName(), player->getGLProgramState(),
                       player->getBlendFunc(), edge_tris, terrain_node->getNodeToWorldTransform(), 1);

    fill_tris.indices = &indices[fill_indices_start];
    fill_tris.indexCount = fill_indices_end;
    fill_tris.verts = &points[0];
    fill_tris.vertCount = points.size();


    cc::Texture2D* fill_t = cc::Director::getInstance()->getTextureCache()->addImage("MossyFill.png");
    fill_t->setTexParameters({ GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT });

    fill_tris_cmd.init(0.0f, fill_t->getName(), player->getGLProgramState(),
                       player->getBlendFunc(), fill_tris, terrain_node->getNodeToWorldTransform(), 1);
}

void state::update(float dt) {
    cc::Director::getInstance()->getRenderer()->addCommand(&fill_tris_cmd);
    cc::Director::getInstance()->getRenderer()->addCommand(&edge_tris_cmd);
    d_node->draw(cc::Director::getInstance()->getRenderer(), terrain_node->getNodeToWorldTransform(), 0);

    player->getPhysicsBody()->applyImpulse(cc::Vec2(0, -10000.0f));
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(-10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_UP_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(0, 40000.0f));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
        player->getPhysicsBody()->applyImpulse(cc::Vec2(0, -40000.0f));
    }
    for (int n = 0; n < peers::peer_list.size(); ++n) {
        msg::send(*peers::peer_list[n]->udp_sock, msg::ByteStream() << _MID->PO_PLAYER_MOVEMENT << 
            (int)player->getPositionX() << (int)player->getPositionY());
    }

    return;

    switch (s) {
        case SERVER_CONNECT_SCREEN:
            sock::update();

            info_label->setPosition(cc::Vec2((cos(time_since_startup) * 40.0f) + 400, 200));

            if (sock::connection_finished) {
                if (sock::connection_error == NO_ERROR) {
                    switch_state(LOGIN_REGISTER_SCREEN);
                }else {
                    info_label->setString("an error occurred while trying to connect: " + SSTR(sock::connection_error));
                }
            }else {
                info_label->setString("connecting to server...");
            }
            break;
    }
}