#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <ui/UIButton.h>

namespace gui {

    namespace cc = cocos2d;

    class Button {

        public:
            Button() { }
            Button(std::string& text, int x, int y);

            cc::ui::Button* button;

            void create(std::string& text, int x, int y);
    };
};

#endif