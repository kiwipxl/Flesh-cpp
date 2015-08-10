#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include <2d/CCLabel.h>
#include <cstdint>

#include "SceneManager.h"

namespace root {

    namespace cc = cocos2d;

    enum State {

        STATE_EMPTY, 
        STATE_SERVER_CONNECT_SCREEN,  
        STATE_LOGIN_REGISTER_SCREEN, 
        STATE_GAME
    };

    extern SceneManager* scene;

    extern State s;
    extern cc::Label* info_label;
    extern float time_since_startup;

    extern void init_root(SceneManager* scene_ref);
    extern void update_state(float dt);
    extern void exit_root();

    extern void switch_state(State new_state, bool force = false);
    extern void on_draw();
};

#endif