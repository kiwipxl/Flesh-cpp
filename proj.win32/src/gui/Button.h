#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <memory>

#include <ui/UIButton.h>

namespace gui {

    namespace cc = cocos2d;

    class Button {

        public:
            ~Button();
            Button(std::string text, int x, int y, int width = 34, int height = 34);

            cc::ui::Button* button;

            void create(std::string text, int x, int y, int width = 34, int height = 34);
            void add_to_scene();
    };

    typedef std::shared_ptr<Button> ButtonPtr;
};

#endif