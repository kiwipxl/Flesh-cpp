#include "Textures.h"

#include <renderer/CCTextureCache.h>
#include <base/CCDirector.h>

#include "debug/Logger.h"

BEGIN_ASSETS_NS

namespace textures {

    using namespace cocos2d;

    //private
    //characters
    Texture2D* duck;
    Texture2D* unit_shadow;

    //ui
    Texture2D* captain_health_bar;
    Texture2D* minion_health_bar;
    Texture2D* footer;
    Texture2D* footer_fire_button;
    Texture2D* inventory_empty;
    Texture2D* inventory_c4;
    Texture2D* inventory_fireworks_gun;
    Texture2D* ui_button_idle;

    //bullets
    Texture2D* c4;
    
    //guns
    Texture2D* laser_machine_gun;

    //entities
    Texture2D* crate;

    //terrain
    Texture2D* jungle_background;

    //menu
    Texture2D* main_screen;
    Texture2D* start_screen;
    Texture2D* options_screen;
    Texture2D* customisation_screen;
    Texture2D* arrow_button;

    void load_image(Texture2D*& tex, std::string file_name) {
        tex = Director::getInstance()->getTextureCache()->addImage(file_name);
    }

    void init() {
        load_image(duck, "characters/duck.png");
        load_image(unit_shadow, "characters/unit_shadow.png");

        load_image(captain_health_bar, "ui/captain_health_bar.png");
        load_image(minion_health_bar, "ui/minion_health_bar.png");
        load_image(footer, "ui/footer.png");
        load_image(footer_fire_button, "ui/footer_fire_button.png");
        load_image(inventory_empty, "ui/inventory_empty.png");
        load_image(inventory_c4, "ui/inventory_c4.png");
        load_image(inventory_fireworks_gun, "ui/inventory_fireworks_gun.png");
        load_image(ui_button_idle, "ui/ui_button_idle.png");

        load_image(c4, "bullets/c4.png");
        load_image(laser_machine_gun, "guns/laser_machine_gun.png");

        load_image(crate, "crate.png");

        load_image(jungle_background, "terrain/jungle_background.png");

        load_image(main_screen, "ui/menu/main_screen.png");
        load_image(start_screen, "ui/menu/start_screen.png");
        load_image(options_screen, "ui/menu/options_screen.png");
        load_image(customisation_screen, "ui/menu/customisation_screen.png");
        load_image(arrow_button, "ui/menu/arrow_button.png");

        log_print_file << "texture assets loaded";
    }
};

END_ASSETS_NS
