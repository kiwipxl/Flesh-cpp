#include "StateManager.h"

#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

#include "assets/Assets.h"
#include "debug/Errors.h"
#include "debug/Logger.h"
#include "input/SimpleInput.h"
#include "map/Ferr2DSystem.h"
#include "map/MapCamera.h"
#include "network/message/Message.h"
#include "network/sockets/SocketManager.h"

#include "states/Game.h"
#include "states/Login.h"
#include "states/Startup.h"

namespace root {

    using namespace cocos2d;

    SceneManager* scene;

    State s = STATE_SERVER_CONNECT_SCREEN;
    Label* info_label;
    float time_since_startup = 0;

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
        states::startup::create_state(s);
        states::login::create_state(s);
        states::game::create_state(s);
    }

    void remove_state(State r_state) {
        s = r_state;
        states::startup::remove_state(s);
        states::login::remove_state(s);
        states::game::remove_state(s);
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

        states::startup::update_state(s);
        states::login::update_state(s);
        states::game::update_state(s);

        network::sock::update();
        input::update_keyboard();
    }
};
