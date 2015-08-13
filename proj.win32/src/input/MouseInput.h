#ifndef _MOUSE_INPUT_H_
#define _MOUSE_INPUT_H_

#include <base/CCEventMouse.h>

namespace input {

    namespace cc = cocos2d;

    extern void init();
    bool get_mouse_pressed(int mouse_button);
    bool get_mouse_down(int mouse_button);
}

#endif