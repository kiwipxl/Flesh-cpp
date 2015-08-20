#include "gui/Button.h"

#include <2d/CCSprite.h>
#include <2d/CCLabelTTF.h>

#include "debug/Logger.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_GUI_NS

//private
std::vector<ButtonPtr> button_list;

//public
ButtonPtr create_button(int _x, int _y) {
    ButtonPtr b(new Button(_x, _y));
    button_list.push_back(b);
    return b;
}

void update_buttons() {
    for (int n = 0; n < button_list.size(); ++n) {
        auto& b = button_list[n];
        if (!b) continue;
        if (!b->is_removal_scheduled()) b->update();
        if (b->is_removal_scheduled()) {
            button_list.erase(button_list.begin() + n, button_list.begin() + n + 1);
            --n;
        }
    }
}

//-- begin Button class --

Button::Button(int _x, int _y) {
    base = Sprite::create();
    base->setPosition(Vec2(_x, _y));

    text = LabelTTF::create("", "fonts/CaviarDreams.ttf", 20);
}

Button::~Button() {

}

void Button::update() {
    update_scheduler();
}

void Button::set_on_click_callback(ButtonClickCallback _callback) {
    click_callback = _callback;
}

void Button::set_size(int _width, int _height) {
    size.width = _width;
    size.height = _height;
    base->setContentSize(size);
}

void Button::set_size(cc::Size _size) {
    size = _size;
    base->setContentSize(size);
}

void Button::set_text(std::string _text) {
    text->setString(_text);
}

void Button::set_text_font_size(int _font_size) {
    text->setFontSize(_font_size);
}

void Button::set_idle_texture(Texture2D* _idle_texture) {
    idle_texture = _idle_texture;
}

void Button::set_hover_texture(Texture2D* _hover_texture) {
    hover_texture = _hover_texture;
}

void Button::set_click_texture(Texture2D* _click_texture) {
    click_texture = _click_texture;
}

void Button::set_disabled_texture(Texture2D* _disabled_texture) {
    disabled_texture = _disabled_texture;
}

void Button::set_textures(Texture2D* _idle_texture, Texture2D* _hover_texture,
                          Texture2D* _click_texture, Texture2D* _disabled_texture) {
    set_idle_texture(_idle_texture);
    set_hover_texture(_hover_texture);
    set_click_texture(_click_texture);
    set_disabled_texture(_disabled_texture);
}
void Button::set_textures(Texture2D* _idle_texture, Texture2D* _click_texture) {
    set_idle_texture(_idle_texture);
    set_click_texture(_click_texture);
}

int Button::get_text_font_size() { return text->getFontSize(); }
std::string Button::get_text_string() { return text->getString(); }

//-- end Button class --

END_GUI_NS
