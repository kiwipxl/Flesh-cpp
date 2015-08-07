#include "StateManager.h"

#include <stdio.h>
#include <cocostudio/CocoStudio.h>
#include <ui/CocosGUI.h>

#include "network/sockets/SocketManager.h"
#include "network/Peers.h"
#include "input/SimpleInput.h"
#include "map/Ferr2DSystem.h"
#include "map/Camera.h"
#include "entities/Unit.h"
#include "debug/Errors.h"
#include "debug/Logger.h"
#include "network/message/Message.h"
#include "assets/Animations.h"

using state::State;

SceneManager* state::scene;

State state::s = state::STATE_SERVER_CONNECT_SCREEN;
Label* state::info_label;
float state::time_since_startup = 0;
Sprite* spinner_sprite;
Node* login_page;
Node* message_box;

ferr2d::Terrain* terrain;
map::Camera* camera;

void create_state(State c_state) {
    using namespace state;

    s = c_state;
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

            sock::setup_tcp_sock();
            break;
        case STATE_LOGIN_REGISTER_SCREEN:
            login_page = CSLoader::createNode("Scene.csb");
            scene->addChild(login_page);
            
            message_box = CSLoader::createNode("message_box.csb");
            {
                ui::ImageView* frame = (ui::ImageView*)message_box->getChildByName("frame_image");

                ui::Text* message_text = (ui::Text*)message_box->getChildByName("message_text");
                message_text->ignoreContentAdaptWithSize(false);
                message_text->setTextAreaSize(Size(frame->getContentSize().width - 40, 120));
                message_text->setPositionY(message_text->getPositionY() - 90);

                ui::Text* message_title = (ui::Text*)message_box->getChildByName("title_text");
                message_title->ignoreContentAdaptWithSize(false);
                message_title->setTextAreaSize(Size(frame->getContentSize().width - 40, message_title->getContentSize().height));
            }
            scene->addChild(message_box);
            break;
        case STATE_GAME:
            entity::test_player = new entity::Unit();
            entity::test_player->player_input = true;
            camera = new map::Camera();
            terrain = new ferr2d::Terrain(*ferr2d::load("terrain.t2d"));
            break;
    }
}

void remove_state(State r_state) {
    using namespace state;

    s = r_state;
    switch (s) {
        case STATE_SERVER_CONNECT_SCREEN:
            scene->removeChild(info_label);
            scene->removeChild(spinner_sprite);
            break;
        case STATE_LOGIN_REGISTER_SCREEN:
            login_page->removeAllChildren();
            break;
    }
}

void state::switch_state(State new_state) {
    remove_state(s);
    create_state(new_state);
}

void state::init(SceneManager* scene_ref) {
    scene = scene_ref;
    scene->setGLProgram(cc::ShaderCache::getInstance()->getGLProgram(cc::GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));

    debug::init_logger();
    assets::animations::init();
    sock::init();
    msg::init();
    input::init();
    create_state(s);
}

void state::update(float dt) {
    time_since_startup += dt;

    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_T)) {
        switch_state(state::STATE_GAME);
    }

    sock::update();
    switch (s) {
        case STATE_SERVER_CONNECT_SCREEN:
            info_label->setPosition(scene->screen_size.width / 2, scene->screen_size.height - 300);

            if (sock::connection_finished) {
                if (sock::connection_err == NO_ERROR) {
                    switch_state(STATE_LOGIN_REGISTER_SCREEN);
                }else {
                    scene->removeChild(spinner_sprite);
                    info_label->setString("an error occurred while trying to connect: " + 
                                            ((sock::connection_err_msg == "") 
                                            ? SSTR(sock::connection_err) 
                                            : sock::connection_err_msg + "(" + SSTR(sock::connection_err) + ")"));
                }
            }else {
                info_label->setString("connecting to server...");
            }
            break;
        case STATE_GAME:
            entity::update_units();
            entity::test_player->update();
            camera->update();
            terrain->draw();

            /*for (int n = 0; n < peers::peer_list.size(); ++n) {
                msg::send(*peers::peer_list[n]->udp_sock, msg::MsgStream() << _MID->PO_PLAYER_MOVEMENT <<
                    (int)entity::test_player->base->getPositionX() << (int)entity::test_player->base->getPositionY() << 
                    (float)entity::test_player->base->getRotation());
            }*/
            break;
    }
}