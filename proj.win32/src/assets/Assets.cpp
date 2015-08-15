#include "Assets.h"

#include "debug/Logger.h"

BEGIN_ASSETS_NS

void init() {
    animations::init();
    csb::init();
    textures::init();
	maps::init();
	particles::init();

    log_print_file << "asset loading completed";
}

END_ASSETS_NS