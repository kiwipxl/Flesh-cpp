#include "SimpleInput.h"

const int input::last_key_code = (int)cc::EventKeyboard::KeyCode::KEY_ENTER;
bool input::keys[last_key_code];
cc::EventListenerKeyboard* input::kb_event;

void input::init() {
    kb_event = cc::EventListenerKeyboard::create();

    kb_event->onKeyPressed = [](cc::EventKeyboard::KeyCode key_code, cc::Event* event) {
        if ((int)key_code >= 0 && (int)key_code <= last_key_code) {
            keys[(int)key_code] = true;
        }
    };

    kb_event->onKeyReleased = [](cc::EventKeyboard::KeyCode key_code, cc::Event* event) {
        if ((int)key_code >= 0 && (int)key_code <= last_key_code) {
            keys[(int)key_code] = false;
        }
    };

    state::scene->getEventDispatcher()->addEventListenerWithFixedPriority(input::kb_event, 10);
}

bool input::key_down(cc::EventKeyboard::KeyCode key_code) {
    if ((int)key_code >= 0 && (int)key_code <= last_key_code) return keys[(int)key_code];
    return false;
}