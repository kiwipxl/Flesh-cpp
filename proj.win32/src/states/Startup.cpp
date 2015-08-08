#include "states/Startup.h"

#include <2d/CCSprite.h>
#include <2d/CCActionInterval.h>
#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>

#include "assets/Assets.h"
#include "network/sockets/SocketManager.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace startup {

    using namespace root;
    using namespace cocos2d;

    //private
    Sprite* spinner_sprite;

    //public externs

    void create_state(State state) {
        switch (state) {
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
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_SERVER_CONNECT_SCREEN:
                scene->removeChild(info_label);
                scene->removeChild(spinner_sprite);
                break;
        }
    }

    void update_state(State state) {
        network::sock::update();
        switch (state) {
            case STATE_SERVER_CONNECT_SCREEN:
                info_label->setPosition(scene->screen_size.width / 2, scene->screen_size.height - 300);

                if (network::sock::connection_finished) {
                    if (network::sock::connection_err == NO_ERROR) {
                        switch_state(STATE_LOGIN_REGISTER_SCREEN);
                    }else {
                        scene->removeChild(spinner_sprite);
                        info_label->setString("an error occurred while trying to connect: " + 
                                                ((network::sock::connection_err_msg == "")
                                                ? SSTR(network::sock::connection_err)
                                                : network::sock::connection_err_msg + "(" + SSTR(network::sock::connection_err) + ")"));
                    }
                }else {
                    info_label->setString("connecting to server...");
                }
                break;
        }
    }
};

END_STATES_NS
