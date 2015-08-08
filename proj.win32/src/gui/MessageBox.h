#ifndef _MESSAGE_BOX_H_
#define _MESSAGE_BOX_H_

/*
handles the creation of custom gui message boxes
note: only one message box can be opened at a time, however a class is used in case this will be changed in the future
*/

#include <memory>

#include <ui/UIText.h>
#include <ui/UIImageView.h>

#include "gui/Button.h"

namespace gui {

    namespace cc = cocos2d;

    class MessageBox {

        public:
            MessageBox(std::string title, std::string message);
            ~MessageBox();

            cc::Node* container;
            cc::ui::ImageView* frame;
            cc::ui::Text* message_text;
            cc::ui::Text* message_title;

            std::vector<Button*> buttons;

            void add_button(Button& button);
            void add_button(std::string button_text, int x, int y);
            void stack_button(std::string button_text);
    };

    typedef std::shared_ptr<MessageBox> MessageBoxPtr;

    extern MessageBoxPtr current_message_box;

    extern MessageBoxPtr show_message_box(std::string title, std::string message);
    extern void hide_message_box();
};

#endif