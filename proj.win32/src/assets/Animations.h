#ifndef _ANIMATIONS_H_
#define _ANIMATIONS_H_

#include <2d/CCAnimation.h>

namespace assets {

    namespace animations {

        using namespace cocos2d;

        extern Vector<SpriteFrame*> spinner_frames;

        extern void init();
        void load_ani_frames(std::string file_name, Vector<SpriteFrame*>& frames, Rect rect);
    };
};

#endif