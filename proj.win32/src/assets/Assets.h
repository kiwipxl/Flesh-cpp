#ifndef _ASSETS_H_
#define _ASSETS_H_

#include "../SceneManager.h"

namespace assets {

    using namespace cc;
    
    extern Vector<SpriteFrame*> spinner_frames;

    extern void init();
    void load_ani_frames(std::string file_name, Vector<SpriteFrame*>& frames, Rect rect);
};

#endif