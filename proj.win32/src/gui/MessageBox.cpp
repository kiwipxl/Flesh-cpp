#include "gui/MessageBox.h"

#include "StateManager.h"
#include "assets/CSB.h"

namespace gui {

    using namespace cocos2d;

    MessageBoxPtr current_message_box;

    //-- begin MessageBox definition --

    MessageBox::MessageBox(std::string& title, std::string& message) {
        root::scene->addChild(assets::csb::message_box);

        frame = (ui::ImageView*)assets::csb::message_box->getChildByName("frame_image");

        message_text = (ui::Text*)assets::csb::message_box->getChildByName("message_text");
        message_text->ignoreContentAdaptWithSize(false);
        message_text->setTextAreaSize(Size(frame->getContentSize().width - 40, 120));
        message_text->setPositionY(message_text->getPositionY() - 90);

        message_title = (ui::Text*)assets::csb::message_box->getChildByName("title_text");
        message_title->ignoreContentAdaptWithSize(false);
        message_title->setTextAreaSize(Size(frame->getContentSize().width - 40, message_title->getContentSize().height));
    }

    void MessageBox::add_button(Button& button) {

    }

    void MessageBox::add_button(std::string& button_text, int x, int y) {

    }

    void stack_button(std::string& button_text) {

    }

    //-- end MessageBox definition --

    MessageBoxPtr& show_message_box(std::string& title, std::string& message) {
        hide_message_box();

        MessageBoxPtr ptr(new MessageBox(title, message));
        current_message_box = ptr;

        return ptr;
    }

    void hide_message_box() {
        root::scene->removeChild(assets::csb::message_box);
    }
};