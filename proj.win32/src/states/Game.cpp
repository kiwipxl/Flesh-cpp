#include "states/Game.h"

#include <physics/CCPhysicsWorld.h>

#include "assets/Assets.h"
#include "entities/Unit.h"
#include "entities/Bullet.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "map/Cam.h"
#include "StateManager.h"
#include "map/Terrain.h"

BEGIN_STATES_NS

namespace game {

    using namespace root;
    using namespace cocos2d;

    //public externs
    map::terrain::TerrainGroup* terrain;
    entities::Unit* current_unit;
    Label* turn_time_label;

    //private
    float dest_zoom = 0;
    float physics_timing = 0;
    const float TIMESTEP = 1.0f / 60.0f;

    void create_state(State state) {
        switch (state) {
            case STATE_GAME:
                turn_time_label = Label::createWithBMFont("fonts/lucida.fnt", "ayyyyyyyyy lmao");
                turn_time_label->setPosition(scene->screen_size.width / 2.0f, scene->screen_size.height - 80);
                turn_time_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
                ui_layer->addChild(turn_time_label, 1);

                terrain = new map::terrain::TerrainGroup(assets::maps::test_terrain.get());
                entities::bullet::init();

                auto unit = new entities::Unit();
                unit->player_input = true;
                unit->base->setPosition(assets::maps::test_terrain->spawn_points[0]);
                auto unit2 = new entities::Unit();
                unit2->base->setPosition(assets::maps::test_terrain->spawn_points[1]);
                current_unit = entities::units[0];

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

    void update_state(State state) {
        switch (state) {
            case STATE_GAME:
                map::camera::map_cam->setPosition(current_unit->base->getPosition());

                if (input::get_mouse_scroll().y <= -1) {
                    dest_zoom -= 20.0f;
                }else if (input::get_mouse_scroll().y >= 1) {
                    dest_zoom += 20.0f;
                }else if (input::key_down(EventKeyboard::KeyCode::KEY_MINUS)) {
                    dest_zoom += 10.0f;
                }else if (input::key_down(EventKeyboard::KeyCode::KEY_EQUAL) || input::get_mouse_scroll().y >= 1) {
                    dest_zoom -= 10.0f;
                }
                dest_zoom -= dest_zoom / 8.0f;
                map::camera::map_cam->setPositionZ(map::camera::map_cam->getPositionZ() + dest_zoom);
                map::camera::static_cam->setPositionZ(map::camera::static_cam->getPositionZ() + dest_zoom);

                entities::update_units();
                entities::bullet::update();
                map::camera::update();
                terrain->draw();

                if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(EventKeyboard::KeyCode::KEY_D)) {
                    terrain->toggle_debug_geometry();

                    root::scene->p_world->setDebugDrawMask(terrain->is_debug_draw_on() ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE);
                }

                scene->p_world->step(TIMESTEP);
                /*physics_timing += delta_time;
                while (physics_timing >= TIMESTEP) {
                scene->p_world->step(TIMESTEP);
                physics_timing -= TIMESTEP;
                }*/

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
