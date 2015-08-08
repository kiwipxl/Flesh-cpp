#include "Assets.h"

#include "debug/Logger.h"

namespace assets {

    void init() {
        animations::init();
        csb::init();
        textures::init();
        maps::init();

        log_print_file << "asset loading completed";
    }
};