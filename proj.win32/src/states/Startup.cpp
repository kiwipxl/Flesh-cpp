#include "states/Startup.h"

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

#include "assets/Assets.h"
#include "network/server/ServerConnection.h"
#include "StateManager.h"
#include "utility/General.h"
#include "utility/Logger.h"

BEGIN_STATES_NS

namespace startup {

    using namespace root;
    using namespace cocos2d;

    //private
    Sprite* spinner_sprite;
    Label* info_label;

    //public externs

    void create_state(State state) {
        switch (state) {
            case STATE_SERVER_CONNECT_SCREEN:
                info_label = Label::createWithBMFont("fonts/felt.fnt", "connecting...");
                info_label->setDimensions(scene->screen_size.width - 40, 400);
                info_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
                info_label->setPosition(scene->screen_size.width / 2, scene->screen_size.height - 300);
                ui_layer->addChild(info_label, 1);

                {
                    auto spinner_animation = Animate::create(Animation::createWithSpriteFrames(assets::animations::spinner_frames, .05f, UINT32_MAX));
                    spinner_sprite = Sprite::create();
                    spinner_sprite->runAction(spinner_animation);
                    spinner_sprite->setPosition(scene->screen_size.width / 2, scene->screen_size.height - 45);
                    ui_layer->addChild(spinner_sprite, 1);
                }

                network::server::setup_tcp_sock([](int err, std::string err_msg) {
                    utility::invoke_main_thread([err, err_msg]() {
                        if (err == NO_ERROR) {
                            info_label->setString("connected!");
                            switch_state(STATE_LOGIN_REGISTER_SCREEN);
                        }else {
                            ui_layer->removeChild(spinner_sprite);
                            info_label->setString(sstream_str("an error occurred while trying to connect: " <<
                                err_msg << " (code: " << err << ")"));
                        }
                    });
                });
                break;
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_SERVER_CONNECT_SCREEN:
                ui_layer->removeChild(info_label);
                ui_layer->removeChild(spinner_sprite);
                if (!network::server::is_connected()) {
                    network::server::close_all_threads();
                }
                break;
        }
    }

    void update_state(State state) {
        switch (state) {
            case STATE_SERVER_CONNECT_SCREEN:
                break;
        }
    }
};

END_STATES_NS
