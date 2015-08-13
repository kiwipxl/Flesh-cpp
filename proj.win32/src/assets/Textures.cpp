#include "Textures.h"

#include <renderer/CCTextureCache.h>
#include <base/CCDirector.h>

#include "debug/Logger.h"

BEGIN_ASSETS_NS

namespace textures {

    using namespace cocos2d;

    //private
    Texture2D* duck;
    Texture2D* test_bullet;

    void load_image(Texture2D*& tex, std::string file_name) {
        tex = Director::getInstance()->getTextureCache()->addImage(file_name);
    }

    void init() {
        load_image(duck, "duck.png");
        load_image(test_bullet, "bullet.png");

        log_print_file << "texture assets loaded";
    }
};

END_ASSETS_NS
