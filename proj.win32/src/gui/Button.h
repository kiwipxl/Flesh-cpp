#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
handles the creation and management of custom gui buttons
*/

#include <memory>
#include <functional>

#include <2d/CCLabelTTF.h>

#include "GUIDefines.h"
#include "entities/EntityScheduler.h"

BEGIN_GUI_NS

#define DEFAULT_BUTTON_WIDTH 50
#define DEFAULT_BUTTON_HEIGHT 34
#define DEFAULT_BUTTON_FONT_SIZE 18

namespace cc = cocos2d;

class Button : public EntityScheduler {

public:
    ~Button();
    Button(int _x, int _y);

    cc::Sprite* base;
    cc::LabelTTF* text;

    void update();

    void set_on_click_callback(ButtonClickCallback _callback);
    void set_size(int _width, int _height);
    void set_size(cc::Size _size);

    void set_pos(cc::Vec2 _pos);
    void set_pos(int _x, int _y);

    void set_text(std::string _text);
    void set_text_font_size(int _font_size);

    void set_idle_texture(cc::Texture2D* _idle_texture);
    void set_hover_texture(cc::Texture2D* _hover_texture);
    void set_click_texture(cc::Texture2D* _click_texture);
    void set_disabled_texture(cc::Texture2D* _disabled_texture);
    void set_textures(cc::Texture2D* _idle_texture, cc::Texture2D* _hover_texture,
                      cc::Texture2D* _click_texture, cc::Texture2D* _disabled_texture);
    void set_textures(cc::Texture2D* _idle_texture, cc::Texture2D* _click_texture);

    int get_text_font_size();
    std::string get_text_string();
    cc::Size get_size() { return size; }
    cc::Vec2 get_pos() { return pos; }

    cc::Texture2D* get_idle_texture() { return idle_texture; }
    cc::Texture2D* get_hover_texture() { return hover_texture; }
    cc::Texture2D* get_click_texture() { return click_texture; }
    cc::Texture2D* get_disabled_texture() { return disabled_texture; }

    bool is_button_down() { return button_down; }

private:
    cc::Vec2 pos;
    cc::Size size;

    cc::Texture2D* idle_texture;
    cc::Texture2D* hover_texture;
    cc::Texture2D* click_texture;
    cc::Texture2D* disabled_texture;

    ButtonClickCallback click_callback = nullptr;
    bool button_down = false;

    void update_text_pos();
};

extern ButtonPtr create_button(int _x, int _y);
extern void update_buttons();

END_GUI_NS

#endif