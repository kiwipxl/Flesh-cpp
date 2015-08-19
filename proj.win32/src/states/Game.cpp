#include "states/Game.h"

#include <physics/CCPhysicsWorld.h>

#include "assets/Assets.h"
#include "entities/bullets/Bullet.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "map/Cam.h"
#include "StateManager.h"
#include "map/Terrain.h"

BEGIN_STATES_NS

namespace game {

    using namespace root;
    using namespace cocos2d;

    //private
    Label* turn_time_label;
    clock_t countdown_start;
    Label* power_label;
    float dest_zoom = 0;

    void update_time() {
        float t = (current_countdown_seconds * 1000) - (clock() - countdown_start);
        if (t <= 0) {
            countdown_start = clock();
            entities::units::next_unit();
            current_countdown_seconds = starting_countdown_seconds;
            return;
        }
        int seconds = t / 1000.0f;
        int ms = clampf(fmod(t, 1000.0f) / 10.0f, 0, 99);
        turn_time_label->setString(sstream_str(seconds << ":" << ms));
    }

    //public
    map::terrain::TerrainGroup* terrain;
    int starting_countdown_seconds = 10;
    int current_countdown_seconds;
    
    void set_countdown_to(float seconds) {
        current_countdown_seconds = seconds;
        countdown_start = clock();
    }

    void set_power_text(float power) {
        power_label->setString(sstream_cstr("power: " << power << "x"));
    }

    void create_state(State state) {
        switch (state) {
            case STATE_GAME:
                glClearColor(0.05f, 0.05f, 0.2f, 0.0f);

                physics::start();

                turn_time_label = Label::createWithBMFont("fonts/felt.fnt", "");
                turn_time_label->setPosition(scene->screen_size.width / 2.0f, scene->screen_size.height - 40);
                turn_time_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
                ui_layer->addChild(turn_time_label, 1);
                countdown_start = clock();
                current_countdown_seconds = starting_countdown_seconds;

                terrain = new map::terrain::TerrainGroup(assets::maps::test_terrain.get());
                entities::bullet::init();

                entities::units::spawn_test_units();

                power_label = Label::createWithBMFont("fonts/felt.fnt", "");
                power_label->setPosition(scene->screen_size.width / 2.0f, 40);
                power_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
                ui_layer->addChild(power_label, 1);

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
                physics::update();
                update_time();

                auto& v = map::camera::map_cam->getPosition();
                auto& vb = entities::units::current_unit->base->getPosition();
                map::camera::map_cam->setPosition(v.x - (v.x - vb.x) / 10.0f, v.y - (v.y - vb.y) / 10.0f);

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

                entities::units::update_all_units();
                entities::bullet::update();
                map::camera::update();
                terrain->draw();

                if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(EventKeyboard::KeyCode::KEY_D)) {
                    terrain->toggle_debug_geometry();

                    root::scene->p_world->setDebugDrawMask(terrain->is_debug_draw_on() ? PhysicsWorld::DEBUGDRAW_ALL : PhysicsWorld::DEBUGDRAW_NONE,
                                                           map::camera::map_cam->getCameraMask());
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
