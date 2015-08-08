#include "input/SimpleInput.h"

#include <base/CCEventListenerKeyboard.h>
#include <base/CCEventDispatcher.h>

#include "StateManager.h"

namespace input {

    const int last_key_code = (int)EventKeyboard::KeyCode::KEY_ENTER;
    bool keys[last_key_code];
    EventListenerKeyboard* kb_event;

    void init() {
        kb_event = EventListenerKeyboard::create();

        kb_event->onKeyPressed = [](EventKeyboard::KeyCode key_code, Event* event) {
            if ((int)key_code >= 0 && (int)key_code <= last_key_code) {
                keys[(int)key_code] = true;
            }
        };

        kb_event->onKeyReleased = [](EventKeyboard::KeyCode key_code, Event* event) {
            if ((int)key_code >= 0 && (int)key_code <= last_key_code) {
                keys[(int)key_code] = false;
            }
        };
        
        root::scene->getEventDispatcher()->addEventListenerWithFixedPriority(kb_event, 10);
    }

    bool key_down(EventKeyboard::KeyCode key_code) {
        if ((int)key_code >= 0 && (int)key_code <= last_key_code) return keys[(int)key_code];
        return false;
    }
};