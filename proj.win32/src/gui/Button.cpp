#include "gui/Button.h"

#include <2d/CCSprite.h>

#include "debug/Logger.h"
#include "StateManager.h"

BEGIN_GUI_NS

using namespace cocos2d;

Button::Button(int _x, int _y) {
    base = Sprite::create();
    base->setTitleFontName("fonts/CaviarDreams.ttf");
    base->setTitleFontSize(font_size);
    base->setTitleText(text);
    base->setPosition(Vec2(x, y));

    base->setContentSize(Size(width, height));
    base->ignoreContentAdaptWithSize(false);
}

Button::~Button() {

}

void Button::set_text(std::string _text) {

}

void Button::set_text_font_size(int _font_size) {

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

END_GUI_NS
