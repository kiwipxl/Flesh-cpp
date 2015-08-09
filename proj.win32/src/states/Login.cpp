#include "states/Login.h"

#include <base/CCScheduler.h>
#include <ui/UIButton.h>
#include <ui/UITextField.h>

#include "assets/Assets.h"
#include "gui/MessageBox.h"
#include "network/message/Message.h"
#include "network/sockets/SocketManager.h"

#include "StateManager.h"
#include "SceneManager.h"

BEGIN_STATES_NS

namespace login {

    using namespace root;
    using namespace cocos2d;
    using namespace network;

    //private
    Node* login_page;
    std::string username_str;
    std::string password_str;

    //public externs
    
    bool verify_account_details() {
        username_str = assets::csb::get_child<ui::TextField>(login_page, "username_input")->getString();
        password_str = assets::csb::get_child<ui::TextField>(login_page, "password_input")->getString();

        if (username_str.length() < 2) {
            gui::show_message_box("account details error",
                "username must be greater than or equal to 3 characters", "OK"); return false;
        }

        if (password_str.length() < 2) {
            gui::show_message_box("account details error",
                "password must be greater than or equal to 3 characters", "OK"); return false;
        }

        return true;
    }

    void create_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                {
                    assets::csb::load_csb(login_page, assets::csb::login_page_name);
                    scene->addChild(login_page);
                    auto login_button = assets::csb::get_child<ui::Button>(login_page, "login_button");
                    auto register_button = assets::csb::get_child<ui::Button>(login_page, "register_button");

                    sock::tcp_serv_sock.add_message_handler(msg::MID_RECV_ATTEMPT_LOGIN_RESULT, [](msg::Message* m) {
                        int result = m->get<int>(0);

                        INVOKE_MAIN_THREAD(result)
                        if (result == 1) {
                            gui::show_message_box("successful", "login successful", "OK");
                        }else {
                            gui::show_message_box("login error", "an unknown error occurred while trying to login. please try again.", "OK");
                        }
                        INVOKE_MAIN_THREAD_END
                    });

                    sock::tcp_serv_sock.add_message_handler(msg::MID_RECV_ATTEMPT_REGISTER_RESULT, [](msg::Message* m) {
                        int result = m->get<int>(0);

                        INVOKE_MAIN_THREAD(result)
                        if (result == 1) {
                            gui::show_message_box("successful", "registered successfully", "OK");
                        }else {
                            gui::show_message_box("login error", "an unknown error occurred while trying to register. please try again.", "OK");
                        }
                        INVOKE_MAIN_THREAD_END
                    });

                    register_button->addClickEventListener([](Ref* r) {
                        if (verify_account_details()) {
                            auto mb = gui::show_loading_message_box("please wait...", "registering...");

                            msg::send(sock::tcp_serv_sock, msg::Stream() << msg::MID_SEND_ATTEMPT_REGISTER << username_str << password_str);
                        }
                    });

                    login_button->addClickEventListener([](Ref* r) {
                        if (verify_account_details()) {
                            auto mb = gui::show_message_box("please wait...", "logging in...", "cancel");
                            mb->add_spinner();

                            msg::send(sock::tcp_serv_sock, msg::Stream() << msg::MID_SEND_ATTEMPT_LOGIN << username_str << password_str);
                        }
                    });
                }
                break;
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                login_page->removeAllChildren();
                login_page->cleanup();
                break;
        }
    }

    void update_state(State state) {

    }
};

END_STATES_NS
