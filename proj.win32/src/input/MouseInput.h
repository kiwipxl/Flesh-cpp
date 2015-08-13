#ifndef _MOUSE_INPUT_H_
#define _MOUSE_INPUT_H_

#include <base/CCEventMouse.h>

namespace input {

    namespace cc = cocos2d;

    extern void init_mouse();
    void update_mouse();

    bool get_mouse_button_pressed       (int mouse_button);
    bool get_mouse_button_down          (int mouse_button);
    cc::Vec2 get_mouse_pos              ();
    cc::Vec2 get_mouse_scroll           ();
}

#endif