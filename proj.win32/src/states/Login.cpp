#include "states/Login.h"

#include "assets/Assets.h"
#include "gui/MessageBox.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace login {

    using namespace root;

    void create_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                scene->addChild(assets::csb::login_page);

                {
                    auto mb = gui::show_message_box("please wait...", "logging in...", "cancel");
                    mb->add_spinner();
                }
                break;
        }
    }

    void remove_state(State state) {
        switch (state) {
            case STATE_LOGIN_REGISTER_SCREEN:
                assets::csb::login_page->removeAllChildren();
                break;
        }
    }

    void update_state(State state) {

    }
};

END_STATES_NS
