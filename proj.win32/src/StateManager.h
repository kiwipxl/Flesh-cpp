#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include <2d/CCLabel.h>
#include <cstdint>

#include "SceneManager.h"

namespace state {

    enum State {
        
        STATE_SERVER_CONNECT_SCREEN,  
        STATE_LOGIN_REGISTER_SCREEN, 
        STATE_GAME
    };

    extern SceneManager* scene;

    extern State s;
    extern cc::Label* info_label;
    extern float time_since_startup;

    extern void init(SceneManager* scene_ref);
    extern void update(float dt);

    extern void switch_state(State new_state);
    extern void on_draw();
};

#endif