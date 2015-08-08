#include "states/Startup.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace startup {

    void create_state(State c_state) {
        switch (s) {
        case STATE_SERVER_CONNECT_SCREEN:
            info_label = Label::createWithBMFont("fonts/lucida.fnt", "connecting...");
            info_label->setDimensions(scene->screen_size.width - 40, 400);
            info_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
            scene->addChild(info_label, 1);

            scene->scheduleUpdate();

            {
                auto spinner_animation = Animate::create(Animation::createWithSpriteFrames(assets::animations::spinner_frames, .05f, UINT32_MAX));
                spinner_sprite = Sprite::create();
                spinner_sprite->runAction(spinner_animation);
                spinner_sprite->setPosition(scene->screen_size.width / 2, scene->screen_size.height - 45);
                scene->addChild(spinner_sprite, 1);
            }

            network::sock::setup_tcp_sock();
            break;
        case STATE_LOGIN_REGISTER_SCREEN:
            scene->addChild(assets::csb::login_page);

            {
                auto mb = gui::show_message_box("please wait...", "logging in...", "cancel");
                mb->add_spinner();
            }
            break;
        case STATE_GAME:
            entities::test_player = new entities::Unit();
            entities::test_player->player_input = true;
            camera = new map::MapCamera();
            terrain = new map::ferr2d::Terrain(*assets::maps::test_terrain);
            break;
        }
    }

    void remove_state(State c_state) {

    }

    void update_state(State c_state) {

    }
};

END_STATES_NS
