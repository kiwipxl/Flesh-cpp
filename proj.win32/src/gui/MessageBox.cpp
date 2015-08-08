#include "gui/MessageBox.h"

#include "StateManager.h"
#include "assets/CSB.h"

namespace gui {

    using namespace cocos2d;

    MessageBoxPtr current_message_box = nullptr;

    //-- begin MessageBox definition --

    MessageBox::MessageBox(std::string title, std::string message) {
        container = assets::csb::message_box;

        root::scene->addChild(container);

        frame = (ui::ImageView*)container->getChildByName("frame_image");

        message_text = (ui::Text*)container->getChildByName("message_text");
        message_text->ignoreContentAdaptWithSize(false);
        message_text->setTextAreaSize(Size(frame->getContentSize().width - 40, 120));
        message_text->setPositionY(message_text->getPositionY() - 90);
        message_text->setString(title);

        message_title = (ui::Text*)container->getChildByName("title_text");
        message_title->ignoreContentAdaptWithSize(false);
        message_title->setTextAreaSize(Size(frame->getContentSize().width - 40, message_title->getContentSize().height));
        message_title->setString(message);
    }

    MessageBox::~MessageBox() {
        root::scene->removeChild(container);
    }

    void MessageBox::add_button(Button& button) {
        container->addChild(button.button);
        buttons.push_back(&button);
    }

    void MessageBox::add_button(std::string button_text, int x, int y) {
        Button* button = new Button(button_text, x, y);
        container->addChild(button->button);
        buttons.push_back(button);
    }

    void MessageBox::stack_button(std::string button_text) {
        int x = ((frame->getPositionX() + (frame->getContentSize().width / 2)) - 55) - (buttons.size() * 85);
        int y = (frame->getPositionY() - (frame->getContentSize().height / 2)) + 35;
        Button* button = new Button(button_text, x, y, 70, 34);
        container->addChild(button->button);
        buttons.push_back(button);
    }

    //-- end MessageBox definition --

    MessageBoxPtr show_message_box(std::string title, std::string message) {
        hide_message_box();

        MessageBoxPtr ptr(new MessageBox(title, message));
        current_message_box = ptr;

        return ptr;
    }

    void hide_message_box() {
        if (current_message_box != nullptr) current_message_box = nullptr;
    }
};