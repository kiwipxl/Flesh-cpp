#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <renderer/CCTexture2D.h>

#include "assets/AssetsDefines.h"

BEGIN_ASSETS_NS

namespace textures {

    namespace cc = cocos2d;

    //characters
    extern cc::Texture2D* duck;
    extern cc::Texture2D* unit_shadow;

    //ui
    extern cc::Texture2D* captain_health_bar;
    extern cc::Texture2D* minion_health_bar;
    extern cc::Texture2D* footer;
    extern cc::Texture2D* footer_fire_button;
    extern cc::Texture2D* inventory_empty;
    extern cc::Texture2D* inventory_c4;
    extern cc::Texture2D* inventory_fireworks_gun;
    extern cc::Texture2D* ui_button_idle;

    //bullets
    extern cc::Texture2D* c4;
    
    //guns
    extern cc::Texture2D* laser_machine_gun;

    //entities
    extern cc::Texture2D* crate;

    //terrain
    extern cc::Texture2D* jungle_background;

    //menu
    extern cc::Texture2D* main_screen;
    extern cc::Texture2D* start_screen;
    extern cc::Texture2D* options_screen;
    extern cc::Texture2D* customisation_screen;
    extern cc::Texture2D* arrow_button;
    extern cc::Texture2D* ui_corner_box;

    extern void init();
};

END_ASSETS_NS

#endif