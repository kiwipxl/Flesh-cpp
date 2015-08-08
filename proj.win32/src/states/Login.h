#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "states/Defines.h"

namespace root {

    enum State;
};

BEGIN_STATES_NS

namespace login {

    extern void create_state(root::State);
    extern void remove_state(root::State);
    extern void update_state(root::State);
};

END_STATES_NS

#endif