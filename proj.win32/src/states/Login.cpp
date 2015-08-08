#include "states/Login.h"

#include <ui/UIButton.h>

#include "assets/Assets.h"
#include "gui/MessageBox.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace login {

    using namespace root;
    using namespace cocos2d;

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
                    login_button->addClickEventListener([](Ref* r) {
                        auto mb = gui::show_message_box("please wait...", "logging in...", "cancel");
                        mb->add_spinner();
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
