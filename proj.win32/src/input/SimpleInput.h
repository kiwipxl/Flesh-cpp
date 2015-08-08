#ifndef _SIMPLE_INPUT_H_
#define _SIMPLE_INPUT_H_

#include <base/CCEventKeyboard.h>

namespace input {

    using namespace cocos2d;

    extern const int last_key_code;
    extern bool keys[];
    extern EventListenerKeyboard* kb_event;

    extern void init();
    extern bool key_down(EventKeyboard::KeyCode key_code);
}

#endif