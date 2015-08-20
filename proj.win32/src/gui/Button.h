#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
class to create gui buttons more easier with extra functionality
*/

#include <memory>
#include <functional>

#include "GUIDefines.h"

BEGIN_GUI_NS

#define DEFAULT_BUTTON_WIDTH 50
#define DEFAULT_BUTTON_HEIGHT 34
#define DEFAULT_BUTTON_FONT_SIZE 18

namespace cc = cocos2d;

class Button {

public:
    ~Button();
    Button(int _x, int _y);

    void set_text(std::string _text);
    void set_text_font_size(int _font_size);

    void set_idle_texture(cc::Texture2D* _idle_texture);
    void set_hover_texture(cc::Texture2D* _hover_texture);
    void set_click_texture(cc::Texture2D* _click_texture);
    void set_disabled_texture(cc::Texture2D* _disabled_texture);
    void set_textures(cc::Texture2D* _idle_texture, cc::Texture2D* _hover_texture,
                      cc::Texture2D* _click_texture, cc::Texture2D* _disabled_texture);
    void set_textures(cc::Texture2D* _idle_texture, cc::Texture2D* _click_texture);

    cc::Sprite* get_base_sprite() { return base; }
    cc::Label* get_text_label() { return text; }

private:
    cc::Sprite* base;
    cc::Label* text;

    cc::Texture2D* idle_texture;
    cc::Texture2D* hover_texture;
    cc::Texture2D* click_texture;
    cc::Texture2D* disabled_texture;
};

END_GUI_NS

#endif