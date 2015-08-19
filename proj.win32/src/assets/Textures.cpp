#include "Textures.h"

#include <renderer/CCTextureCache.h>
#include <base/CCDirector.h>

#include "debug/Logger.h"

BEGIN_ASSETS_NS

namespace textures {

    using namespace cocos2d;

    //private
    Texture2D* duck;
    Texture2D* captain_health_bar;
    Texture2D* minion_health_bar;
    Texture2D* unit_shadow;
    Texture2D* c4;
    Texture2D* laser_machine_gun;

    void load_image(Texture2D*& tex, std::string file_name) {
        tex = Director::getInstance()->getTextureCache()->addImage(file_name);
    }

    void init() {
        load_image(duck, "characters/duck.png");
        load_image(captain_health_bar, "ui/captain_health_bar.png");
        load_image(minion_health_bar, "ui/minion_health_bar.png");
        load_image(unit_shadow, "characters/unit_shadow.png");
        load_image(c4, "bullets/c4.png");
        load_image(laser_machine_gun, "guns/laser_machine_gun.png");

        log_print_file << "texture assets loaded";
    }
};

END_ASSETS_NS
