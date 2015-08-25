#include "StateManager.h"

#include <renderer/CCGLProgram.h>
#include <renderer/CCGLProgramCache.h>
#include <base/CCDirector.h>
#include <ui/CocosGUI.h>
#include "cocos2d.h"

#include "assets/Assets.h"
#include "debug/Errors.h"
#include "entities/items/Weapon.h"
#include "gui/MessageBox.h"
#include "gui/Button.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "map/Cam.h"
#include "network/message/Message.h"
#include "network/server/ServerConnection.h"
#include "physics/Physics.h"
#include "utility/Logger.h"

#include "states/Game.h"
#include "states/Login.h"
#include "states/Menu.h"
#include "states/Startup.h"

namespace root {

    using namespace cocos2d;

    //public
    SceneManager* scene;
    State s = STATE_EMPTY;
    Node* ui_layer;
    Node* map_layer;
    Node* bg_layer;

    float time_since_startup = 0;
    float delta_time = 0;
    
    //private
    bool created_init_state = false;

    void create_state(State, bool = false);
    void remove_state(State, bool = false);

    void init_root(SceneManager* scene_ref) {
        scene = scene_ref;
        scene->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        srand(time(NULL));

        ui_layer = Node::create();
        scene->addChild(ui_layer, 3);
        map_layer = Node::create();
        scene->addChild(map_layer, 2);
        bg_layer = Node::create();
        scene->addChild(bg_layer, 1);
        map::camera::init();

        utility::init_logger();
        assets::init();
        network::server::init();
        network::msg::init();
        input::init_keyboard();
        input::init_mouse();
        physics::init();
        entities::items::init_weapons();

        s = STATE_SERVER_CONNECT_SCREEN;
    }

    void create_state(State c_state, bool force) {
        if (!force && s == c_state) { log_info << "cannot create state " << c_state << " when on same state " << s; return; }

        s = c_state;
        states::startup::create_state(s);
        states::login::create_state(s);
        states::game::create_state(s);
        states::menu::create_state(s);

        log_info << "created state " << c_state;
    }

    void remove_state(State r_state, bool force) {
        if (!force && s == r_state) { log_info << "cannot remove state " << r_state << " when on same state " << s; return; }

        s = r_state;
        states::startup::remove_state(s);
        states::login::remove_state(s);
        states::game::remove_state(s);
        states::menu::remove_state(s);

        log_info << "removed state " << r_state;
    }

    void switch_state(State new_state, bool force) {
        if (!force && s == new_state) { log_info << "cannot switch to state " << new_state << " when on state " << s; return; }

        log_info << "switching to state " << new_state << " from state " << s;

        remove_state(s, true);
        create_state(new_state, true);

        log_info << "switched to state " << new_state << " from state " << s;
    }

    void update_state(float dt) {
        ui_layer->setCameraMask((u_short)CameraFlag::USER1);
        map_layer->setCameraMask((u_short)CameraFlag::USER3);
        bg_layer->setCameraMask((u_short)CameraFlag::USER2);

        time_since_startup += dt;
        delta_time = dt;

        if (!created_init_state) { created_init_state = true; create_state(s, true); }

        //debugging stuff
        if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(EventKeyboard::KeyCode::KEY_N)) {
            //Director::getInstance()->getOpenGLView()->setFrameSize(800, 600);
            switch_state((State)((int)s + 1));
        }

        states::startup::update_state(s);
        states::login::update_state(s);
        states::game::update_state(s);
        states::menu::update_state(s);
        
        network::server::update();
        gui::update_buttons();
        input::update_keyboard();
        input::update_mouse();
    }

    void exit_root() {
        remove_state(s, true);
        network::server::close_all_threads();
    }
};
