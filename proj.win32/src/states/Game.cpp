#include "states/Game.h"

#include <physics/CCPhysicsWorld.h>

#include "assets/Assets.h"
#include "entities/Unit.h"
#include "entities/Bullet.h"
#include "input/KeyboardInput.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace game {

    using namespace root;
    using namespace cocos2d;

    //public externs
    map::ferr2d::Terrain* terrain;
    map::MapCamera* camera;

    //private

    void create_state(State state) {
        switch (state) {
            case STATE_GAME:
                entities::test_player = new entities::Unit();
                entities::test_player->player_input = true;
                camera = new map::MapCamera();
                terrain = new map::ferr2d::Terrain(*assets::maps::test_terrain);
                entities::bullet::init();

                break;
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_GAME:
                entities::bullet::deinit();

                break;
        }
    }

    float physics_timing = 0;
    const float TIMESTEP = 1.0f / 60.0f;
    void update_state(State state) {
        switch (state) {
            case STATE_GAME:
                if (input::key_down(EventKeyboard::KeyCode::KEY_MINUS)) {
                    camera->cam->setPositionZ(camera->cam->getPositionZ() + 10.0f);
                }else if (input::key_down(EventKeyboard::KeyCode::KEY_EQUAL)) {
                    camera->cam->setPositionZ(camera->cam->getPositionZ() - 10.0f);
                }

                //scene->p_world->step(delta_time);
                /*physics_timing += delta_time;
                while (physics_timing >= TIMESTEP) {
                    scene->p_world->step(TIMESTEP);
                    physics_timing -= TIMESTEP;
                }*/

                entities::update_units();
                entities::bullet::update();
                camera->update();
                entities::test_player->update();
                terrain->draw();

                if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(EventKeyboard::KeyCode::KEY_D)) {
                    terrain->toggle_debug_geometry();

                    root::scene->p_world->setDebugDrawMask(terrain->is_debug_draw_on() ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
                }

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
