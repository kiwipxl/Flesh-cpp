#include "input/MouseInput.h"

#include <base/CCEventDispatcher.h>
#include <base/CCEventMouse.h>
#include <base/CCEventListenerMouse.h>

#include "StateManager.h"

namespace input {

    using namespace cocos2d;

    //private
    EventListenerMouse* mouse_listener;

    struct MouseResult {

        bool down = false;
        bool pressed = false;
        Vec2 pos;
        Vec2 scroll;
    };
    MouseResult mouse_left;
    MouseResult mouse_right;
    MouseResult mouse_undefined;

    MouseResult& get_mouse_result(int button) {
        switch (button) {
            case MOUSE_BUTTON_LEFT:
                return mouse_left;
                break;
            case MOUSE_BUTTON_RIGHT:
                return mouse_right;
                break;
            default:
                return mouse_undefined;
        }
    }

    void init() {
        mouse_listener = EventListenerMouse::create();

        mouse_listener->onMouseDown = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            get_mouse_result(em->getMouseButton()).down = true;
            get_mouse_result(em->getMouseButton()).pressed = true;
        };

        mouse_listener->onMouseUp = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            get_mouse_result(em->getMouseButton()).down = false;
            get_mouse_result(em->getMouseButton()).pressed = false;
        };

        mouse_listener->onMouseMove = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            get_mouse_result(em->getMouseButton()).pos.x = em->getCursorX();
            get_mouse_result(em->getMouseButton()).pos.y = em->getCursorY();
        };

        mouse_listener->onMouseScroll = [](Event* e) {
            EventMouse* em = (EventMouse*)e;
            get_mouse_result(em->getMouseButton()).scroll.x = em->getScrollX();
            get_mouse_result(em->getMouseButton()).scroll.y = em->getScrollY();
        };

        root::scene->getEventDispatcher()->addEventListenerWithFixedPriority(mouse_listener, 10);
    }

    bool get_mouse_pressed(int mouse_button) {
        return get_mouse_result(mouse_button).pressed;
    }

    bool get_mouse_down(int mouse_button) {
        return get_mouse_result(mouse_button).down;
    }
    
    Vec2 get_mouse_pos(int mouse_button) {
        return get_mouse_result(mouse_button).pos;
    }
    
    Vec2 get_mouse_scroll(int mouse_button) {
        return get_mouse_result(mouse_button).scroll;
    }

    void update_mouse() {
        mouse_left.pressed = false;
    }
};