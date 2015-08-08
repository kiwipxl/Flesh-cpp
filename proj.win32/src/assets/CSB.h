#ifndef _CSB_H_
#define _CSB_H_

#include <2d/CCNode.h>

#include "debug/Logger.h"

namespace assets {

    namespace csb {

        namespace cc = cocos2d;

        extern std::string login_page_name;
        extern std::string message_box_name;
        extern cc::Node* message_box;

        extern void init();

        extern void load_csb(cc::Node*& cs_node, std::string file_name);

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