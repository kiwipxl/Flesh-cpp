#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <renderer/CCTexture2D.h>

#include "assets/AssetsDefines.h"

BEGIN_ASSETS_NS

namespace textures {

    namespace cc = cocos2d;

    extern cc::Texture2D* duck;
    extern cc::Texture2D* test_bullet;
    extern cc::Texture2D* cone;
    extern cc::Texture2D* captain_health_bar;
    extern cc::Texture2D* minion_health_bar;
    extern cc::Texture2D* unit_shadow;
    extern cc::Texture2D* c4;
    extern cc::Texture2D* laser_machine_gun;

    extern void init();
};

END_ASSETS_NS

#endif