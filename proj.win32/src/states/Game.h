#ifndef _GAME_H_
#define _GAME_H_

#include "map/Terrain.h"
#include "states/Defines.h"

namespace root {

    enum State;
};

BEGIN_STATES_NS

namespace game {

    extern map::terrain::TerrainGroup* terrain;
    extern int starting_countdown_seconds;
    extern int current_countdown_seconds;

    extern void create_state(root::State);
    extern void remove_state(root::State);
    extern void update_state(root::State);

    extern void set_countdown_to(float seconds);
};

END_STATES_NS

#endif