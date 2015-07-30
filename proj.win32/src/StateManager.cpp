#include "StateManager.h"
#include "debug/Errors.h"
#include "network/sockets/SocketManager.h"
#include "input/SimpleInput.h"
#include "network/Peers.h"
#include "map/Ferr2DSystem.h"
#include "entities/Unit.h"
#include "map/Camera.h"
#include <stdio.h>

using state::State;

SceneManager* state::scene;

State state::s = state::STATE_SERVER_CONNECT_SCREEN;
cc::LabelBMFont* state::info_label;
float state::time_since_startup = 0;
cc::TextFieldDelegate username_input;

ferr2d::Terrain* terrain;
map::Camera* camera;

void create_state(State c_state) {
    using namespace state;

    s = c_state;
    switch (s) {
        case STATE_SERVER_CONNECT_SCREEN:
            info_label = cc::LabelBMFont::create("", "fonts/lucida.fnt");
            info_label->setString("connecting...");
            scene->addChild(info_label, 1);

            scene->scheduleUpdate();

            sock::setup_tcp_sock();
            break;
        case STATE_LOGIN_REGISTER_SCREEN:
            break;
        case STATE_GAME:
            entity::test_player = new entity::Unit();
            entity::test_player->player_input = true;
            camera = new map::Camera();
            terrain = new ferr2d::Terrain(*ferr2d::load("terrain.t2d"));
            break;
    }
}

void remove_state(State r_state) {
    using namespace state;

    s = r_state;
    switch (s) {
        case STATE_SERVER_CONNECT_SCREEN:
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
    scene->setGLProgram(cc::ShaderCache::getInstance()->getGLProgram(cc::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

    sock::init();
    input::init();

    create_state(s);
}

void state::update(float dt) {
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_T)) {
        switch_state(state::STATE_GAME);
    }

    switch (s) {
        case STATE_SERVER_CONNECT_SCREEN:
            sock::update();

            info_label->setPosition(cc::Vec2((cos(time_since_startup) * 40.0f) + 400, 200));

            if (sock::connection_finished) {
                if (sock::connection_error == NO_ERROR) {
                    switch_state(STATE_GAME);
                }else {
                    info_label->setString("an error occurred while trying to connect: " + SSTR(sock::connection_error));
                }
            }else {
                info_label->setString("connecting to server...");
            }
            break;
        case STATE_GAME:
            entity::update_units();
            entity::test_player->update();
            camera->update();
            terrain->draw();

            for (int n = 0; n < peers::peer_list.size(); ++n) {
                msg::send(*peers::peer_list[n]->udp_sock, msg::ByteStream() << _MID->PO_PLAYER_MOVEMENT <<
                    (int)entity::test_player->base->getPositionX() << (int)entity::test_player->base->getPositionY() << 
                    (float)entity::test_player->base->getRotation());
            }
            break;
    }
}