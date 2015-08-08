#include "gui/Button.h"

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
};