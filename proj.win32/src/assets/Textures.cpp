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
    Texture2D* cone;
    Texture2D* captain_health_bar;
    Texture2D* minion_health_bar;
    Texture2D* unit_shadow;
    Texture2D* c4;

    void load_image(Texture2D*& tex, std::string file_name) {
        tex = Director::getInstance()->getTextureCache()->addImage(file_name);
    }

    void init() {
        load_image(duck, "duck.png");
        load_image(test_bullet, "bullet.png");
        load_image(cone, "cone.png");
        load_image(captain_health_bar, "captain_health_bar.png");
        load_image(minion_health_bar, "minion_health_bar.png");
        load_image(unit_shadow, "unit_shadow.png");
        load_image(c4, "c4.png");

        log_print_file << "texture assets loaded";
    }
};

END_ASSETS_NS
