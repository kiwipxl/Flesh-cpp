#ifndef _GAME_H_
#define _GAME_H_

#include "map/Ferr2DSystem.h"
#include "map/MapCamera.h"
#include "states/Defines.h"

namespace root {

    enum State;
};

BEGIN_STATES_NS

namespace game {

    extern map::ferr2d::Terrain* terrain;
    extern map::MapCamera* camera;

    extern void create_state(root::State);
    extern void remove_state(root::State);
    extern void update_state(root::State);
};

END_STATES_NS

#endif