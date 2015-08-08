#include "gui/MessageBox.h"

#include <2d/CCActionInterval.h>

#include "StateManager.h"
#include "assets/Animations.h"
#include "assets/CSB.h"

namespace gui {

    using namespace cocos2d;

    MessageBoxPtr current_message_box = nullptr;

    //-- begin MessageBox definition --

    MessageBox::MessageBox(std::string title, std::string message) {
        container = assets::csb::message_box;

        root::scene->addChild(container);

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
        root::scene->removeChild(container);
    }

    void MessageBox::add_button(Button& button) {
        container->addChild(button.button);
        buttons.push_back(&button);
    }

    void MessageBox::add_button(std::string button_text, int x, int y, int font_size, ButtonClickCallback on_click) {
        Button* button = new Button(button_text, x, y, DEFAULT_BUTTON_WIDTH, DEFAULT_BUTTON_HEIGHT, font_size, on_click);
        container->addChild(button->button);
        buttons.push_back(button);
    }

    void MessageBox::stack_button(std::string button_text, int font_size, ButtonClickCallback on_click) {
        int x = ((frame->getPositionX() + (frame->getContentSize().width / 2)) - 55) - (buttons.size() * 85);
        int y = (frame->getPositionY() - (frame->getContentSize().height / 2)) + 35;
        Button* button = new Button(button_text, x, y, 70, 34, font_size, on_click);
        container->addChild(button->button);
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

    void MessageBox::close() {
        close_message_box();
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
        //set smart ptr to nullptr which will call the deconstructor of the message box and delete the pointer reference
        if (current_message_box != nullptr) current_message_box = nullptr;
    }
    
    void close_message_box_callback(Ref* r) {
        close_message_box();
    }
};