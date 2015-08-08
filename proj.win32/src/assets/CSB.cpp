#include "CSB.h"

#include <exception>

#include <cocostudio/CocoStudio.h>

#include "debug/Logger.h"

namespace assets {

    namespace csb {

        using namespace cocos2d;

        std::string login_page_name = "Scene.csb";
        std::string message_box_name = "message_box.csb";
        Node* message_box;

        void init() {
            load_csb(message_box, message_box_name);

            log_print_file << "csb assets loaded";
        }
        
        void load_csb(Node*& cs_node, std::string file_name) {
            try {
                cs_node = CSLoader::createNode(file_name);
            }catch (const std::exception& e) {
                f_assert(sstream << "CSB could not be loaded (" << file_name << "): " << e.what());
            }catch (...) {
                f_assert(sstream << "unknown error occurred while loading CSB (" << file_name << ")");
            }

            cs_node->retain();
        }
    };
};