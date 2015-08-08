#ifndef _MESSAGE_BOX_H_
#define _MESSAGE_BOX_H_

#include <memory>

namespace gui {

    class MessageBox {

        public:
    };

    typedef std::unique_ptr<MessageBox> MessageBoxPtr;

    MessageBoxPtr show_message_box();
    void hide_message_box();
};

#endif