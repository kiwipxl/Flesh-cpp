#ifndef _KEYBOARD_INPUT_H_
#define _KEYBOARD_INPUT_H_

#include <base/CCEventKeyboard.h>

namespace input {

    namespace cc = cocos2d;

    struct Key {

        bool down = false;
        bool pressed = false;
    };

    extern Key keys[];

    extern void init();
    extern bool key_down(cc::EventKeyboard::KeyCode key_code);
    extern bool key_pressed(cc::EventKeyboard::KeyCode key_code);
    extern void update_keyboard();
}

#endif