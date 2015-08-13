#ifndef _TEXTURES_H_
#define _TEXTURES_H_

#include <renderer/CCTexture2D.h>

#include "assets/AssetsDefines.h"

BEGIN_ASSETS_NS

namespace textures {

    namespace cc = cocos2d;

    extern cc::Texture2D* duck;
    extern cc::Texture2D* test_bullet;

    extern void init();
};

END_ASSETS_NS

#endif