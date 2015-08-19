#include "states/Game.h"

#include <physics/CCPhysicsWorld.h>

#include "assets/Assets.h"
#include "entities/bullets/Bullet.h"
#include "entities/bullets/BulletGroup.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "gui/GameGUI.h"
#include "input/KeyboardInput.h"
#include "map/Cam.h"
#include "StateManager.h"
#include "map/Terrain.h"

BEGIN_STATES_NS

namespace game {

    using namespace root;
    using namespace cocos2d;

    //private

    //public
    map::terrain::TerrainGroup* terrain;

    void create_state(State state) {
        switch (state) {
            case STATE_GAME:
                glClearColor(0.05f, 0.05f, 0.2f, 0.0f);

                physics::start();

                terrain = new map::terrain::TerrainGroup(assets::maps::test_terrain.get());
                entities::bullets::init();

                entities::units::spawn_test_units();
                gui::game::init();

                break;
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_GAME:
                entities::bullets::deinit();

                break;
        }
    }

    void update_state(State state) {
        switch (state) {
            case STATE_GAME:
                gui::game::update();
                map::camera::update_game_cam();

                entities::units::update_all_units();
                entities::bullets::update();
                terrain->draw();

                if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(EventKeyboard::KeyCode::KEY_D)) {
                    terrain->toggle_debug_geometry();

                    root::scene->p_world->setDebugDrawMask(terrain->is_debug_draw_on() ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE,
                                                           map::camera::map_cam->getCameraMask());
                }

                physics::update();

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
