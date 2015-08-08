#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include <2d/CCAnimation.h>

namespace assets {

    namespace animations {

        namespace cc = cocos2d;

        extern cc::Vector<cc::SpriteFrame*> spinner_frames;

        extern void init();
    };
};

#endif