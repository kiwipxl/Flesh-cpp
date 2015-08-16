#include "input/MouseInput.h"

#include <base/CCEventDispatcher.h>
#include <base/CCEventMouse.h>
#include <base/CCEventListenerMouse.h>

#include "debug/Logger.h"

#include "StateManager.h"

namespace input {

    using namespace cocos2d;

    //private
    EventListenerMouse* mouse_listener;

    struct MouseButton {

        bool down = false;
        bool pressed = false;
    };

    struct MouseData {

        Vec2 pos;
        Vec2 scroll;

        MouseButton button_left;
        MouseButton button_right;
        MouseButton button_undefined;
    } mouse;

    MouseButton& get_mouse_button(int button) {
        switch (button) {
            case MOUSE_BUTTON_LEFT:
                return mouse.button_left;
                break;
            case MOUSE_BUTTON_RIGHT:
                return mouse.button_right;
                break;
            default:
                return mouse.button_undefined;
        }
    }

    void init_mouse() {
        mouse_listener = EventListenerMouse::create();

        mouse_listener->onMouseDown = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            get_mouse_button(em->getMouseButton()).down = true;
            get_mouse_button(em->getMouseButton()).pressed = true;
        };

        mouse_listener->onMouseUp = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            get_mouse_button(em->getMouseButton()).down = false;
            get_mouse_button(em->getMouseButton()).pressed = false;
        };

        mouse_listener->onMouseMove = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            mouse.pos.x = em->getCursorX();
            mouse.pos.y = em->getCursorY();
        };

        mouse_listener->onMouseScroll = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            mouse.scroll.x = em->getScrollX();
            mouse.scroll.y = em->getScrollY();
        };

        root::scene->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_listener, 10);
    }

    bool get_mouse_button_pressed(int mouse_button) {
        return get_mouse_button(mouse_button).pressed;
    }

    bool get_mouse_button_down(int mouse_button) {
        return get_mouse_button(mouse_button).down;
    }
    
    Vec2 get_mouse_pos() {
        return mouse.pos;
    }
    
    Vec2 get_mouse_scroll() {
        return mouse.scroll;
    }

    void update_mouse() {
        mouse.button_left.pressed = false;
        mouse.button_right.pressed = false;
        mouse.scroll = { 0, 0 };
    }
};