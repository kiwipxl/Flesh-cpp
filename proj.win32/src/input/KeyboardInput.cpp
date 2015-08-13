#include "input/KeyboardInput.h"

#include <base/CCEventListenerKeyboard.h>
#include <base/CCEventDispatcher.h>

#include "StateManager.h"

namespace input {

    using namespace cocos2d;

    //private
    const int last_key_code = (int)EventKeyboard::KeyCode::KEY_ENTER;
    EventListenerKeyboard* kb_listener;

    //public
    Key keys[last_key_code];

    void init_keyboard() {
        kb_listener = EventListenerKeyboard::create();

        kb_listener->onKeyPressed = [](EventKeyboard::KeyCode key_code, Event* event) {
            if ((int)key_code >= 0 && (int)key_code <= last_key_code) {
                keys[(int)key_code].down = true;
                keys[(int)key_code].pressed = true;
            }
        };

        kb_listener->onKeyReleased = [](EventKeyboard::KeyCode key_code, Event* event) {
            if ((int)key_code >= 0 && (int)key_code <= last_key_code) {
                keys[(int)key_code].down = false;
                keys[(int)key_code].pressed = false;
            }
        };
        
        root::scene->getEventDispatcher()->addEventListenerWithFixedPriority(kb_listener, 10);
    }

    bool key_down(EventKeyboard::KeyCode key_code) {
        if ((int)key_code >= 0 && (int)key_code <= last_key_code) return keys[(int)key_code].down;
        return false;
    }
    
    bool key_pressed(EventKeyboard::KeyCode key_code) {
        if ((int)key_code >= 0 && (int)key_code <= last_key_code) return keys[(int)key_code].pressed;
        return false;
    }

    void update_keyboard() {
        for (int n = 0; n < last_key_code; ++n) {
            keys[n].pressed = false;
        }
    }
};