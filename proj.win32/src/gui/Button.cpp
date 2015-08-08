#include "gui/Button.h"

#include "debug/Logger.h"
#include "StateManager.h"

namespace gui {

    using namespace cocos2d;

    Button::Button(std::string text, int x, int y) {
        create(text, x, y);
    }

    void Button::create(std::string text, int x, int y) {
        button = ui::Button::create();
        button->setTitleText(text);
        button->setPosition(Vec2(x, y));
    }

    void Button::add_to_scene() {
        cf_assert(!button, sstream << "cannot add NULL button to scene");
        root::scene->addChild(button);
    }

    Button::~Button() {
        if (button) root::scene->removeChild(button);
    }
};