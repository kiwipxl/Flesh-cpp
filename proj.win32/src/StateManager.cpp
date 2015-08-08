#include "StateManager.h"

#include <stdio.h>
#include <2d/CCActionInterval.h>
#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

#include "assets/Assets.h"
#include "debug/Errors.h"
#include "debug/Logger.h"
#include "entities/Unit.h"
#include "gui/MessageBox.h"
#include "input/SimpleInput.h"
#include "map/Ferr2DSystem.h"
#include "map/MapCamera.h"
#include "network/message/Message.h"
#include "network/sockets/SocketManager.h"
#include "network/Peers.h"

#include "states/Game.h"
#include "states/Login.h"
#include "states/Startup.h"

namespace root {

    using namespace cocos2d;

    SceneManager* scene;

    State s = STATE_SERVER_CONNECT_SCREEN;
    Label* info_label;
    float time_since_startup = 0;
    Sprite* spinner_sprite;

    map::ferr2d::Terrain* terrain;
    map::MapCamera* camera;

    void create_state(State);
    void remove_state(State);
    void switch_state(State);

    void init_root(SceneManager* scene_ref) {
        scene = scene_ref;
        scene->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

        debug::init_logger();
        assets::init();
        network::sock::init();
        network::msg::init();
        input::init();
        create_state(s);
    }

    void create_state(State c_state) {
        s = c_state;
        states::startup::create_state(c_state);
        states::login::create_state(c_state);
        states::game::create_state(c_state);
    }

    void remove_state(State r_state) {
        s = r_state;
        switch (s) {
            case STATE_SERVER_CONNECT_SCREEN:
                scene->removeChild(info_label);
                scene->removeChild(spinner_sprite);
                break;
            case STATE_LOGIN_REGISTER_SCREEN:
                assets::csb::login_page->removeAllChildren();
                break;
        }
    }

    void switch_state(State new_state) {
        remove_state(s);
        create_state(new_state);
    }

    void update_state(float dt) {
        time_since_startup += dt;

        if (input::key_pressed(EventKeyboard::KeyCode::KEY_T)) {
            switch_state(STATE_GAME);
        }
        
        network::sock::update();
        switch (s) {
            case STATE_SERVER_CONNECT_SCREEN:
                info_label->setPosition(scene->screen_size.width / 2, scene->screen_size.height - 300);

                if (network::sock::connection_finished) {
                    if (network::sock::connection_err == NO_ERROR) {
                        switch_state(STATE_LOGIN_REGISTER_SCREEN);
                    }else {
                        scene->removeChild(spinner_sprite);
                        info_label->setString("an error occurred while trying to connect: " + 
                                                ((network::sock::connection_err_msg == "")
                                                ? SSTR(network::sock::connection_err)
                                                : network::sock::connection_err_msg + "(" + SSTR(network::sock::connection_err) + ")"));
                    }
                }else {
                    info_label->setString("connecting to server...");
                }
                break;
            case STATE_GAME:
                entities::update_units();
                entities::test_player->update();
                camera->update();
                terrain->draw();

                /*for (int n = 0; n < peers::peer_list.size(); ++n) {
                    msg::send(*peers::peer_list[n]->udp_sock, msg::MsgStream() << _MID->PO_PLAYER_MOVEMENT <<
                        (int)entities::test_player->base->getPositionX() << (int)entities::test_player->base->getPositionY() << 
                        (float)entities::test_player->base->getRotation());
                }*/
                break;
        }
        input::update_keyboard();
    }
};
