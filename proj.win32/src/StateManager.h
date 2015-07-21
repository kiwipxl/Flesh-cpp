#ifndef _STATE_MANAGER_H_
#define _STATE_MANAGER_H_

#include <cstdint>
#include "SceneManager.h"

namespace state {

    enum State {

        TCP_SERVER_CONNECT,
        UDP_SERVER_CONNECT,
        SERVER_CONNECTED, 
        SERVER_CONNECTION_FAILED
    };

    extern SceneManager* scene;

    extern State s;
    extern cc::LabelBMFont* label;
    extern float time_since_startup;

    extern void init(SceneManager* scene_ref);
    extern void update(float dt);

    extern void switch_state(State new_state);
};

#endif