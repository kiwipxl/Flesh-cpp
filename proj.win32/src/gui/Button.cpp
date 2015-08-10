#include "gui/Button.h"

#include "debug/Logger.h"
#include "StateManager.h"

namespace gui {

    using namespace cocos2d;

    Button::Button(std::string text, int x, int y, int width, int height, int font_size, ButtonClickCallback on_click) {
        create(text, x, y, width, height, font_size, on_click);
    }

    Button::Button(std::string text, int x, int y, int font_size, ButtonClickCallback on_click) {
        create(text, x, y, DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT, font_size, on_click);
    }

    void Button::create(std::string text, int x, int y, int width, int height, int font_size, ButtonClickCallback on_click) {
        button = ui::Button::create("Button_Disable.png");
        button->setTitleFontName("fonts/CaviarDreams.ttf");
        button->setTitleFontSize(font_size);
        button->setTitleText(text);
        button->setPosition(Vec2(x, y));
        button->setContentSize(Size(width, height));
        button->ignoreContentAdaptWithSize(false);
        if (on_click != nullptr) button->addClickEventListener(on_click);
    }

    void Button::add_to_scene() {
        cf_assert(!button, sstream_cstr("cannot add NULL button to scene"));
        root::scene->addChild(button);
    }

    Button::~Button() {
        if (button) root::scene->removeChild(button);
    }
};