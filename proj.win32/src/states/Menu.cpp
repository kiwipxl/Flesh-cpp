#include "states/Menu.h"

#include <2d/CCSprite.h>

#include "assets/Textures.h"
#include "gui/Button.h"
#include "network/sockets/SocketManager.h"

#include "StateManager.h"

BEGIN_STATES_NS

namespace menu {

    using namespace root;
    using namespace cocos2d;

    //private
    Sprite* main_menu;
    Sprite* options_menu;
    Sprite* customisations_menu;
    gui::ButtonPtr lbutton;
    gui::ButtonPtr rbutton;
    Node* menu_node;

    bool scrolling = false;
    float scroll_dest_x = 0;

    void create_menu(Sprite*& s, Texture2D& t) {
        s = Sprite::createWithTexture(&t);
        s->setScaleX(scene->screen_size.width / s->getContentSize().width);
        s->setScaleY(scene->screen_size.height / s->getContentSize().height);
        s->setAnchorPoint(Vec2(0, 0));
        menu_node->addChild(s, 1);
    }

    void scroll_left() {
        if (scrolling) return;

        scrolling = true;
        scroll_dest_x += scene->screen_size.width;
    }

    void scroll_right() {
        if (scrolling) return;

        scrolling = true;
        scroll_dest_x -= scene->screen_size.width;
    }

    //public

    void create_state(State state) {
        switch (state) {
        case STATE_MENU:
            menu_node = Node::create();
            ui_layer->addChild(menu_node, 1);

            create_menu(main_menu, *assets::textures::main_screen);
            create_menu(options_menu, *assets::textures::options_screen);
            create_menu(customisations_menu, *assets::textures::customisation_screen);

            options_menu->setPositionX(-scene->screen_size.width);
            customisations_menu->setPositionX(scene->screen_size.width);

            lbutton = gui::create_button(32, scene->screen_size.height / 2.0f);
            lbutton->set_on_click_callback(scroll_left);
            ui_layer->addChild(lbutton->base, 1);
            rbutton = gui::create_button(scene->screen_size.width - 32, scene->screen_size.height / 2.0f);
            rbutton->set_on_click_callback(scroll_right);
            ui_layer->addChild(rbutton->base, 1);

            break;
        }
    }

    void remove_state(State state) {
        switch (state) {
        case STATE_MENU:
            ui_layer->removeChild(main_menu);
            ui_layer->removeChild(options_menu);
            ui_layer->removeChild(customisations_menu);
            break;
        }
    }

    void update_state(State state) {
        switch (state) {
        case STATE_MENU:
            if (scrolling) {
                float x = menu_node->getPositionX() - (menu_node->getPositionX() - scroll_dest_x) / 8.0f;
                menu_node->setPosition(x, menu_node->getPositionY());
                if (x >= scroll_dest_x - 1 && x <= scroll_dest_x + 1) {
                    scrolling = false;
                    menu_node->setPositionX(scroll_dest_x);

                    for (auto c : menu_node->getChildren()) {
                        if (scroll_dest_x + c->getPositionX() < -scene->screen_size.width * 1.5f) {
                            c->setPositionX(c->getPositionX() + (scene->screen_size.width * 3.0f));
                        }else if (scroll_dest_x + c->getPositionX() > scene->screen_size.width * 1.5f) {
                            c->setPositionX(c->getPositionX() - (scene->screen_size.width * 3.0f));
                        }
                    }
                }
            }

            break;
        }
    }
};

END_STATES_NS
