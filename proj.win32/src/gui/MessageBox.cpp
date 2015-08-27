#include "gui/MessageBox.h"

#include <2d/CCActionInterval.h>

#include "assets/Animations.h"
#include "assets/CSB.h"
#include "assets/Textures.h"
#include "StateManager.h"

BEGIN_GUI_NS

using namespace cocos2d;

//private

//public
MessageBoxPtr current_message_box = nullptr;

//-- begin MessageBox definition --

MessageBox::MessageBox(std::string title, std::string message) {
    assets::csb::load_csb(container, assets::csb::message_box_name);
    //container = assets::csb::message_box;

    root::ui_layer->addChild(container, 10);

    frame = assets::csb::get_child<ui::ImageView>(container, "frame_image");

    message_title = assets::csb::get_child<ui::Text>(container, "title_text");
    message_title->ignoreContentAdaptWithSize(false);
    message_title->setTextAreaSize(Size(frame->getContentSize().width - 40, message_title->getContentSize().height));
    message_title->setString(title);

    message_text = assets::csb::get_child<ui::Text>(container, "message_text");
    message_text->ignoreContentAdaptWithSize(false);
    message_text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    message_text->setTextAreaSize(Size(frame->getContentSize().width - 40, 120));
    message_text->setPositionY(message_text->getPositionY() - 95);
    message_text->setString(message);
}

MessageBox::~MessageBox() {
    close();
}

void MessageBox::close() {
    if (!closed) {
        for (auto& b : buttons) {
            container->removeChild(b->base);
            b = NULL;
        }
        buttons.clear();
        root::ui_layer->removeChild(container);
        closed = true;
    }
}

void MessageBox::add_button(ButtonPtr button) {
    container->addChild(button->base);
    buttons.push_back(button);
}

void MessageBox::add_button(std::string _button_text, int _x, int _y, int _font_size, ButtonClickCallback _on_click_callback) {
    ButtonPtr button = create_button(_x, _y);
    button->set_text(_button_text);
    button->set_text_font_size(_font_size);
    button->set_on_click_callback(_on_click_callback);
    button->set_idle_texture(assets::textures::ui_button_idle);
    container->addChild(button->base);
    buttons.push_back(button);
}

void MessageBox::stack_button(std::string _button_text, int _font_size, ButtonClickCallback _on_click_callback) {
    int x = ((frame->getPositionX() + (frame->getContentSize().width / 2)) - 90) - (buttons.size() * 85);
    int y = (frame->getPositionY() - (frame->getContentSize().height / 2)) + 18;

    ButtonPtr button = create_button(x, y);
    button->set_text(_button_text);
    button->set_text_font_size(_font_size);
    button->set_on_click_callback(_on_click_callback);
    button->set_idle_texture(assets::textures::ui_button_idle);
    button->set_size(70, 34);
    container->addChild(button->base);
    buttons.push_back(button);
}
    
Sprite* MessageBox::add_spinner() {
    auto spinner_animation = Animate::create(Animation::createWithSpriteFrames(assets::animations::spinner_frames, .05f, UINT32_MAX));
    auto spinner_sprite = Sprite::create();
    spinner_sprite->runAction(spinner_animation);
    spinner_sprite->setPosition(frame->getPositionX(), frame->getPositionY() - 20);
    container->addChild(spinner_sprite, 1);
    return spinner_sprite;
}

//-- end MessageBox definition --
    
MessageBoxPtr show_mb(std::string title, std::string message) {
    close_message_box();

    MessageBoxPtr ptr(new MessageBox(title, message));
    current_message_box = ptr;

    return ptr;
}

MessageBoxPtr show_message_box(std::string title, std::string message) {
    auto ptr = show_mb(title, message);

    return ptr;
}

MessageBoxPtr show_message_box(std::string title, std::string message, std::string button_title, ButtonClickCallback on_click) {
    auto ptr = show_mb(title, message);
    ptr->stack_button(button_title, DEFAULT_BUTTON_FONT_SIZE, on_click);

    return ptr;
}

MessageBoxPtr show_loading_message_box(std::string title, std::string message) {
    auto ptr = show_mb(title, message);
    ptr->add_spinner();

    return ptr;
}

void close_message_box() {
    if (current_message_box != nullptr) current_message_box.reset();
}

void close_message_box_callback() {
    close_message_box();
}

END_GUI_NS
