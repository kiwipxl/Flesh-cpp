#include "gui/MessageBox.h"

#include <ui/CocosGUI.h>

#include "StateManager.h"
#include "assets/CSB.h"

namespace gui {

    using namespace cocos2d;

    MessageBoxPtr show_message_box() {
        hide_message_box();

        MessageBoxPtr ptr = MessageBoxPtr(new MessageBox());

        root::scene->addChild(assets::csb::message_box);

        ui::ImageView* frame = (ui::ImageView*)assets::csb::message_box->getChildByName("frame_image");

        ui::Text* message_text = (ui::Text*)assets::csb::message_box->getChildByName("message_text");
        message_text->ignoreContentAdaptWithSize(false);
        message_text->setTextAreaSize(Size(frame->getContentSize().width - 40, 120));
        message_text->setPositionY(message_text->getPositionY() - 90);

        ui::Text* message_title = (ui::Text*)assets::csb::message_box->getChildByName("title_text");
        message_title->ignoreContentAdaptWithSize(false);
        message_title->setTextAreaSize(Size(frame->getContentSize().width - 40, message_title->getContentSize().height));

        return ptr;
    }

    void hide_message_box() {
        root::scene->removeChild(assets::csb::message_box);
    }
};