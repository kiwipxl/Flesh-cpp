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

    //public externs

    void create_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                {
                    assets::csb::load_csb(login_page, assets::csb::login_page_name);
                    scene->addChild(login_page);
                    auto login_button = assets::csb::get_child<ui::Button>(login_page, "login_button");

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

                    login_button->addClickEventListener([](Ref* r) {
                        auto mb = gui::show_message_box("please wait...", "logging in...", "cancel");
                        mb->add_spinner();

                        auto username_input = assets::csb::get_child<ui::TextField>(login_page, "username_input");
                        auto password_input = assets::csb::get_child<ui::TextField>(login_page, "password_input");
                        std::string str1 = username_input->getString();

                        msg::send(sock::tcp_serv_sock, msg::Stream() << msg::MID_SEND_ATTEMPT_LOGIN << username_input->getString() << password_input->getString());
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
