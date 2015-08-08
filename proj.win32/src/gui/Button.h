#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
class to create gui buttons more easier with extra functionality
*/

#include <memory>
#include <functional>

#include <ui/UIButton.h>

#define DEFAULT_BUTTON_WIDTH 50
#define DEFAULT_BUTTON_HEIGHT 34

namespace gui {

    namespace cc = cocos2d;

    typedef cc::ui::Widget::ccWidgetClickCallback ButtonClickCallback;

    class Button {

        public:
            ~Button();
            Button(std::string text, int x, int y, int width = DEFAULT_BUTTON_WIDTH, int height = DEFAULT_BUTTON_HEIGHT, 
                   ButtonClickCallback on_click = nullptr);
            Button(std::string text, int x, int y, ButtonClickCallback on_click = nullptr);

            cc::ui::Button* button;

            void create(std::string text, int x, int y, int width, int height, 
                        ButtonClickCallback on_click = nullptr);
            void add_to_scene();
    };

    typedef std::shared_ptr<Button> ButtonPtr;
};

#endif