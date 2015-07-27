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

cc::TrianglesCommand tri_cmd;
cc::TrianglesCommand::Triangles tri;
u_short* indices;

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

    FILE* f = fopen("terrain.t2d", "r");

    if (f != NULL) {
        const int max_buf = 1024;
        char buf[max_buf];
        fgets(buf, max_buf, f);

        float v_x = 0;
        float v_y = 0;
        std::string temp = "";
        bool s = false;
        for (int n = 12; n < max_buf; ++n) {
            char c = buf[n];
            if (c == '\0') break;
            if (c == ',') {
                if (s) {
                    v_y = std::stof(temp);
                    cc::V3F_C4B_T2F v;
                    v.vertices.x = v_x * 100.0f;
                    v.vertices.y = v_y * 100.0f;
                    v.colors = cc::Color4B(255, 255, 255, 255);
                    v.texCoords.u = 0;
                    v.texCoords.v = 0;
                    points.push_back(v);

                    //if (points.size() >= 12) break;
                }else v_x = std::stof(temp);
                temp = "";
                s = !s;
            }else if (c == ' ') {
                break;
            }else {
                temp += c;
            }
        }
    }

    fclose(f);

    player = cc::Sprite::create("HelloWorld.png");
    player->setPosition(player->getContentSize().width, player->getContentSize().height);
    player->setScale(.25f);
    player->retain();
    //scene->addChild(player, 1);

    auto physics_body = cc::PhysicsBody::createBox(cc::Size(player->getContentSize().width * player->getScaleX(), 
                                                            player->getContentSize().height * player->getScaleY()));
    player->setPhysicsBody(physics_body);

    scene->p_world->setDebugDrawMask(cc::PhysicsWorld::DEBUGDRAW_ALL);

    d_node = cc::DrawNode::create();
    //dphysics_body->setGravityEnable(false);
    //dphysics_body->setDynamic(false);
    scene->addChild(d_node, 1);

    indices = new u_short[points.size()] { 0,1,2,3,1,0,3,4,1,3,5,4,3,6,5,3,7,6,7,3,8,9,10,11,12,9,11,13,14,14,13,13,14,15,16,16,15,15,16,17,18,19,20,17,19,21,22,22,21,21,22,23,24,24,23,23,24,25,26,27,28,25,27,29,30,31,32,29,31,33,34,35,36,33,35,37,38,39,40,37,39,41,42,43,44,41,43,45,46,46,45,45,46,47,48,48,47,47,48,49,50,51,52,49,51,53,54,55,56,53,55,57,58,59,60,57,59,61,62,63,64,61,63,65,66,67,68,65,67,69,70,71,72,69,71,73,74,75,76,73,75,77,78,79,80,77,79,81,82,83,84,81,83, };

    tri.indices = indices;
    tri.indexCount = points.size();
    tri.verts = &points[0];
    tri.vertCount = points.size();

    tri_cmd.init(0.0f, player->getTexture()->getName(), player->getGLProgramState(), 
        player->getBlendFunc(), tri, player->getNodeToWorldTransform(), 1);
}

void state::update(float dt) {
    cc::Director::getInstance()->getRenderer()->addCommand(&tri_cmd);

    player->getPhysicsBody()->applyForce(cc::Vec2(0, -2000.0f));
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