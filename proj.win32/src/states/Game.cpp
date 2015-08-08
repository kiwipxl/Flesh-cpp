#include "states/Game.h"

#include "assets/Assets.h"
#include "entities/Unit.h"
#include "map/Ferr2DSystem.h"
#include "map/MapCamera.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace game {

    using namespace root;

    //private
    map::ferr2d::Terrain* terrain;
    map::MapCamera* camera;

    //public externs

    void create_state(State state) {
        switch (state) {
            case STATE_GAME:
                entities::test_player = new entities::Unit();
                entities::test_player->player_input = true;
                camera = new map::MapCamera();
                terrain = new map::ferr2d::Terrain(*assets::maps::test_terrain);
                break;
        }
    }

    void remove_state(State state) {

    }

    void update_state(State state) {
        switch (state) {
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
    }
};

END_STATES_NS
