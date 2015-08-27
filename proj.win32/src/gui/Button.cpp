#include "gui/Button.h"

#include <2d/CCSprite.h>
#include <2d/CCLabelTTF.h>

#include "assets/Textures.h"
#include "utility/Logger.h"
#include "input/MouseInput.h"
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
    text = LabelTTF::create("", "fonts/CaviarDreams.ttf", DEFAULT_BUTTON_FONT_SIZE);
    base->addChild(text, 1);

    text->setHorizontalAlignment(TextHAlignment::CENTER);
    text->setVerticalAlignment(TextVAlignment::CENTER);

    set_idle_texture(assets::textures::ui_button_idle);
    set_size(DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT);
    set_pos(_x, _y);
}

Button::~Button() {

}

void Button::update() {
    update_scheduler();

    Vec2 mpos = input::get_mouse_pos();
    if (mpos.x >= pos.x - (scaled_size.width / 2.0f) && mpos.y <= pos.y + (scaled_size.height / 2.0f) &&
        mpos.x <= pos.x + (scaled_size.width / 2.0f) && mpos.y >= pos.y - (scaled_size.height / 2.0f)) {
        if (input::get_mouse_button_down(MOUSE_BUTTON_LEFT)) {
            button_down = true;
        }else if (button_down) {
            if (click_callback) click_callback();
        }
    }

    if (!input::get_mouse_button_down(MOUSE_BUTTON_LEFT)) {
        button_down = false;
    }
}

void Button::set_on_click_callback(ButtonClickCallback _callback) { click_callback = _callback; }

void Button::update_text_pos() {
    text->setPosition(Vec2(0, 0));
    text->setAnchorPoint(Vec2(0, 0));
}
void Button::update_size() { set_size(size.width, size.height); }

void Button::set_size(int _width, int _height) {
    size.width = _width;
    size.height = _height;

    _width *= scale.x;
    _height *= scale.y;

    scaled_size.width = _width;
    scaled_size.height = _height;

    base->setScaleX(_width / base->getContentSize().width);
    base->setScaleY(_height / base->getContentSize().height);

    text->setScaleX(base->getContentSize().width / _width);
    text->setScaleY(base->getContentSize().height / _height);
    text->setDimensions(size);
    update_text_pos();

    base->setContentSize(size);
}

void Button::set_size(cc::Size _size) { set_size(_size.width, _size.height); }

void Button::set_scale(float _scale_x, float _scale_y) { scale.x = _scale_x; scale.y = _scale_y; }
void Button::set_scale_x(float _scale_x) { scale.x = _scale_x; }
void Button::set_scale_y(float _scale_y) { scale.y = _scale_y; }

void Button::set_pos(cc::Vec2 _pos) {
    pos = _pos;
    base->setPosition(pos);
    update_text_pos();
}

void Button::set_pos(int _x, int _y) {
    pos.x = _x; pos.y = _y;
    base->setPosition(pos);
    update_text_pos();
}

void Button::set_text(std::string _text) {
    text->setString(_text);
}

void Button::set_text_font_size(int _font_size) {
    text->setFontSize(_font_size);
}

void Button::set_idle_texture(Texture2D* _idle_texture) {
    idle_texture = _idle_texture;
    base->setTexture(idle_texture);
    base->setTextureRect(Rect(0, 0, base->getTexture()->getContentSize().width, base->getTexture()->getContentSize().height));
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
