#ifndef _STARTUP_H_
#define _STARTUP_H_

#include "states/Defines.h"

BEGIN_STATES_NS

enum State;

namespace startup {

    extern void create_state(State c_state);
    extern void remove_state(State c_state);
    extern void update_state(State c_state);
};

END_STATES_NS

#endif