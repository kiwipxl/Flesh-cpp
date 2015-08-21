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

    //bullets
    extern cc::Texture2D* c4;
    
    //guns
    extern cc::Texture2D* laser_machine_gun;

    //entities
    extern cc::Texture2D* crate;

    //terrain
    extern cc::Texture2D* jungle_background;

    extern void init();
};

END_ASSETS_NS

#endif