#include "gui/Button.h"

#include "debug/Logger.h"
#include "StateManager.h"

namespace gui {

    using namespace cocos2d;

    Button::Button(std::string text, int x, int y, int width, int height) {
        create(text, x, y, width, height);
    }

    void Button::create(std::string text, int x, int y, int width, int height) {
        button = ui::Button::create("Button_Disable.png");
        button->setTitleText(text);
        button->setPosition(Vec2(x, y));
        button->setContentSize(Size(width, height));
        button->ignoreContentAdaptWithSize(false);
    }

    void Button::add_to_scene() {
        cf_assert(!button, sstream << "cannot add NULL button to scene");
        root::scene->addChild(button);
    }

    Button::~Button() {
        if (button) root::scene->removeChild(button);
    }
};