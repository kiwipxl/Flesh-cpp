#include "StateManager.h"
#include "debug/Errors.h"
#include "network/sockets/SocketManager.h"
#include "input/SimpleInput.h"
#include "network/Peers.h"
#include "map/Ferr2DSystem.h"
#include "entities/Player.h"
#include <stdio.h>

using state::State;

SceneManager* state::scene;

State state::s = state::SERVER_CONNECT_SCREEN;
cc::LabelBMFont* state::info_label;
float state::time_since_startup = 0;
cc::TextFieldDelegate username_input;

cc::Camera* camera;

ferr2d::Terrain* terrain;
entity::Player* player;

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


    player = new entity::Player();
    terrain = new ferr2d::Terrain(*ferr2d::load("terrain.t2d"));

    auto spritePos = cc::Vec3(scene->screen_size.width / 2 + 0,
        scene->screen_size.height / 2 + 0,
        0);

    // position the sprite on the center of the screen
    //this is the layer, when adding camera to it, all its children will be affect only when you set the second parameter to true
    scene->setCameraMask((unsigned short)cc::CameraFlag::USER1, true);
    // add the sprite as a child to this layer

    camera = cc::Camera::createPerspective(60, (float)scene->screen_size.width / scene->screen_size.height, 1.0, 1000);
    camera->setCameraFlag(cc::CameraFlag::USER1);
    //the calling order matters, we should first call setPosition3D, then call lookAt.
    camera->setPosition3D(spritePos + cc::Vec3(0, 0, 800));
    camera->lookAt(spritePos, cc::Vec3(0.0, 1.0, 0.0));
    scene->addChild(camera);
}

void state::update(float dt) {
    player->update();

    cc::Vec3 pos;
    pos.x = player->base->getPositionX();
    pos.y = player->base->getPositionY();
    camera->setPosition3D(pos + cc::Vec3(0, 0, 600));
    camera->lookAt(pos, cc::Vec3(0.0, 1.0, 0.0));

    terrain->draw();

    player->pbody->applyImpulse(cc::Vec2(0, -10000.0f));
    player->pbody->setVelocityLimit(500.0f);
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        player->pbody->applyImpulse(cc::Vec2(10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        player->pbody->applyImpulse(cc::Vec2(-10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_UP_ARROW)) {
        player->pbody->applyImpulse(cc::Vec2(0, 40000.0f));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
        player->pbody->applyImpulse(cc::Vec2(0, -40000.0f));
    }

    for (int n = 0; n < peers::peer_list.size(); ++n) {
        msg::send(*peers::peer_list[n]->udp_sock, msg::ByteStream() << _MID->PO_PLAYER_MOVEMENT << 
            (int)player->base->getPositionX() << (int)player->base->getPositionY());
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