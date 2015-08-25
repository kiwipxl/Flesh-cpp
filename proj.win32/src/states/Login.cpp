#include "states/Login.h"

#include <regex>

#include <ui/UIButton.h>
#include <ui/UITextField.h>

#include "assets/Assets.h"
#include "gui/MessageBox.h"
#include "network/message/Message.h"
#include "network/message/Stream.h"
#include "network/message/MID.h"
#include "network/server/ServerConnection.h"
#include "network/sockets/Socket.h"
#include "utility/ThreadSchedule.h"

#include "StateManager.h"
#include "SceneManager.h"

BEGIN_STATES_NS

namespace login {

    using namespace root;
    using namespace cocos2d;
    using namespace network;

    //private
    Node* login_page;
    ui::TextField* username_input;
    std::string username_str;
    ui::TextField* password_input;
    std::string password_str;
    const std::regex input_regex("^[a-zA-Z0-9]+$");
    Sprite* background;

    enum LoginResult {
        LOGIN_RESULT_SUCCESS,
        LOGIN_RESULT_INCORRECT_USER_OR_PASS,
        LOGIN_RESULT_INVALID_FORMAT,
        LOGIN_RESULT_UNKNOWN_ERROR
    };

    enum RegisterResult {
        REGISTER_RESULT_SUCCESS,
        REGISTER_RESULT_USER_ALREADY_EXISTS,
        REGISTER_RESULT_INVALID_FORMAT,
        REGISTER_RESULT_UNKNOWN_ERROR
    };

    #define MIN_USERNAME_LEN 3
    #define MAX_USERNAME_LEN 16
    #define MIN_PASSWORD_LEN 3
    #define MAX_PASSWORD_LEN 16

    //public externs
    
    bool verify_account_details() {
        username_str = username_input->getString();
        password_str = password_input->getString();

        if (username_str.length() < MIN_USERNAME_LEN) {
            gui::show_message_box("account details error",
                                   sstream_cstr("username must be greater than or equal to " << MIN_USERNAME_LEN << " characters"), "OK"); return false;
        }
        if (password_str.length() < MIN_PASSWORD_LEN) {
            gui::show_message_box("account details error",
                                   sstream_cstr("password must be greater than or equal to " << MIN_PASSWORD_LEN << " characters"), "OK"); return false;
        }
        if (username_str.length() > MAX_USERNAME_LEN) {
            gui::show_message_box("account details error",
                                   sstream_cstr("username must be less than or equal to " << MAX_USERNAME_LEN << " characters"), "OK"); return false;
        }
        if (password_str.length() > MAX_PASSWORD_LEN) {
            gui::show_message_box("account details error",
                                   sstream_cstr("password must be less than or equal to " << MAX_PASSWORD_LEN << " characters"), "OK"); return false;
        }

        if (!std::regex_search(username_str, input_regex)) {
            gui::show_message_box("account details error", 
                                  "username cannot contain any special characters, only alphabetical and numerical", "OK");
            return false;
        }

        if (!std::regex_search(password_str, input_regex)) {
            gui::show_message_box("account details error",
                                  "password cannot contain any special characters, only alphabetical and numerical", "OK");
            return false;
        }

        return true;
    }

    void create_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                {
                    background = Sprite::createWithTexture(assets::textures::start_screen);
                    background->setScaleX(scene->screen_size.width / background->getContentSize().width);
                    background->setScaleY(scene->screen_size.height / background->getContentSize().height);
                    background->setAnchorPoint(Vec2(0, 0));
                    ui_layer->addChild(background, 1);

                    assets::csb::load_csb(login_page, assets::csb::login_page_name);
                    ui_layer->addChild(login_page, 1);
                    auto login_button = assets::csb::get_child<ui::Button>(login_page, "login_button");
                    auto register_button = assets::csb::get_child<ui::Button>(login_page, "register_button");

                    username_input = assets::csb::get_child<ui::TextField>(login_page, "username_input");
                    username_input->setMaxLength(MAX_USERNAME_LEN);
                    password_input = assets::csb::get_child<ui::TextField>(login_page, "password_input");
                    password_input->setMaxLength(MAX_PASSWORD_LEN);

                    server::tcp_sock.add_message_handler(msg::MID_RECV_ATTEMPT_LOGIN_RESULT, [](msg::Message* m) {
                        LoginResult result = (LoginResult)m->get<int>(0);

                        utility::invoke_main_thread([result]() {
                            switch (result) {
                                case LOGIN_RESULT_SUCCESS:
                                    gui::show_message_box("successful", "logged in successfully", "OK");
                                    break;
                                case LOGIN_RESULT_INCORRECT_USER_OR_PASS:
                                    gui::show_message_box("login error", "incorrect username or password. please try again.", "OK");
                                    break;
                                case LOGIN_RESULT_INVALID_FORMAT:
                                    gui::show_message_box("login error", "invalid format error occurred. please try again.", "OK");
                                    break;
                                default:
                                    gui::show_message_box("login error", "an unknown error occurred while trying to login. please try again.", "OK");
                                    break;
                            }
                        });
                    });
                    
                    server::tcp_sock.add_message_handler(msg::MID_RECV_ATTEMPT_REGISTER_RESULT, [](msg::Message* m) {
                        RegisterResult result = (RegisterResult)m->get<int>(0);

                        utility::invoke_main_thread([result]() {
                            switch (result) {
                                case REGISTER_RESULT_SUCCESS:
                                    gui::show_message_box("successful", "registered successfully", "OK");
                                    break;
                                case REGISTER_RESULT_USER_ALREADY_EXISTS:
                                    gui::show_message_box("register error", "username already exists. please try another name.", "OK");
                                    break;
                                case REGISTER_RESULT_INVALID_FORMAT:
                                    gui::show_message_box("register error", "invalid format error occurred. please try again.", "OK");
                                    break;
                                default:
                                    gui::show_message_box("register error", "an unknown error occurred while trying to register. please try again.", "OK");
                                    break;
                            }
                        });
                    });

                    register_button->addClickEventListener([](Ref* r) {
                        if (verify_account_details()) {
                            auto mb = gui::show_loading_message_box("please wait...", "registering...");

                            msg::send(server::tcp_sock, msg::Stream() << msg::MID_SEND_ATTEMPT_REGISTER << username_str << password_str);
                        }
                    });

                    login_button->addClickEventListener([](Ref* r) {
                        if (verify_account_details()) {
                            auto mb = gui::show_message_box("please wait...", "logging in...", "cancel");
                            mb->add_spinner();

                            msg::send(server::tcp_sock, msg::Stream() << msg::MID_SEND_ATTEMPT_LOGIN << username_str << password_str);
                        }
                    });
                }
                break;
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                ui_layer->removeChild(background);
                ui_layer->removeChild(login_page);
                login_page->cleanup();
                gui::close_message_box();
                break;
        }
    }

    void update_state(State state) {

    }
};

END_STATES_NS
