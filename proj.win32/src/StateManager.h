#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include <cstdint>
#include "SceneManager.h"

namespace state {

    enum State {

        SERVER_CONNECT_SCREEN,  
        LOGIN_REGISTER_SCREEN
    };

    extern SceneManager* scene;

    extern State s;
    extern cc::LabelBMFont* info_label;
    extern float time_since_startup;

    extern void init(SceneManager* scene_ref);
    extern void update(float dt);

    extern void switch_state(State new_state);
};

#endif