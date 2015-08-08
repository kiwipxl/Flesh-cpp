#ifndef _SIMPLE_INPUT_H_
#define _SIMPLE_INPUT_H_

#include <base/CCEventKeyboard.h>

namespace input {

    namespace cc = cocos2d;

    extern const int last_key_code;
    extern bool keys[];
    extern cc::EventListenerKeyboard* kb_event;

    extern void init();
    extern bool key_down(cc::EventKeyboard::KeyCode key_code);
}

#endif