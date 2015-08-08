#include "CSB.h"

#include <cocostudio/CocoStudio.h>

#include "debug/Logger.h"

namespace assets {

    namespace csb {

        Node* login_page;
        Node* message_box;

        void init() {
            login_page = CSLoader::createNode("Scene.csb");
            login_page->retain();
            message_box = CSLoader::createNode("message_box.csb");
            message_box->retain();

            log_print_file << "csb assets loaded";
        }
    };
};