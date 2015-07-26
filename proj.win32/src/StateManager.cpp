#include "StateManager.h"
#include "debug/Errors.h"
#include "network/sockets/SocketManager.h"
#include "input/SimpleInput.h"
#include "network/Peers.h"

using state::State;

SceneManager* state::scene;

State state::s = state::SERVER_CONNECT_SCREEN;
cc::LabelBMFont* state::info_label;
float state::time_since_startup = 0;
cc::TextFieldDelegate username_input;

cc::Sprite* state::player;
cc::DrawNode* d_node;

void create_state(State c_state) {
    using namespace state;

    s = c_state;
    switch (s) {
        case SERVER_CONNECT_SCREEN:
            info_label = cc::LabelBMFont::create("", "fonts/lucida.fnt");
            info_label->setString("connecting...");
            scene->addChild(info_label, 1);

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

    sock::init();
    input::init();

    create_state(s);

    player = cc::Sprite::create("HelloWorld.png");
    player->setPosition(player->getContentSize().width, player->getContentSize().height);
    player->setScale(.25f);
    scene->addChild(player, 1);

    auto physics_body = cc::PhysicsBody::createBox(cc::Size(player->getContentSize().width * player->getScaleX(), 
                                                            player->getContentSize().height * player->getScaleY()));
    player->setPhysicsBody(physics_body);
    physics_body->setLinearDamping(1.0f);

    const cc::Vec2* verts = new cc::Vec2[4] { cc::Vec2(0, 200), cc::Vec2(700, 180), cc::Vec2(700, 0), cc::Vec2(0, 0) };
    scene->p_world->setDebugDrawMask(cc::PhysicsWorld::DEBUGDRAW_ALL);
    auto dphysics_body = cc::PhysicsBody::create();
    dphysics_body->addShape(cc::PhysicsShapePolygon::create(verts, 3));

    d_node = cc::DrawNode::create();
    d_node->drawPolygon(verts, 3, cc::Color4F(1, 0, 1, 1), 1, cc::Color4F(1, 1, 1, 1));
    d_node->setPhysicsBody(dphysics_body);
    dphysics_body->setGravityEnable(false);
    dphysics_body->setDynamic(false);
    scene->addChild(d_node, 1);
}

void state::update(float dt) {
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

    switch (s) {
        case SERVER_CONNECT_SCREEN:
            sock::update();

            time_since_startup += dt;
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