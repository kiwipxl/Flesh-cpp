#ifndef _CSB_H_
#define _CSB_H_

#include <2d/CCNode.h>

#include "debug/Logger.h"

namespace assets {

    namespace csb {

        namespace cc = cocos2d;

        extern cc::Node* login_page;
        extern cc::Node* message_box;

        extern void init();

        //gets a child from a csb node and logs an error if it doesn't exist
        //example usage: assets::csb::get_child<ui::Button>(node, name)
        template <typename T> T* get_child(cc::Node* csb_node, std::string name) {
            T* t;
            if ((t = csb_node->getChildByName<T*>(name)) == NULL) {
                f_assert(sstream << "(" << name << ") could not be found in csb node. halting...");
            }else {
                return t;
            }
            return NULL;
        }
    };
};

#endif