#include "Assets.h"

#include "../debug/Logger.h"

namespace assets {

    Vector<SpriteFrame*> spinner_frames;

    void init() {
        load_ani_frames("spinner_sheet2.png", spinner_frames, Rect(0, 0, 34, 34));

        log_print_file << "finished assets loading";
    }

    void load_ani_frames(std::string file_name, Vector<SpriteFrame*>& frames, Rect rect) {
        Texture2D* texture = TextureCache::sharedTextureCache()->addImage(file_name);
        if (texture == NULL) {
            log_print_file << "could not load texture (" << file_name << ")";
            assert(!(sstream << "could not load texture (" << file_name << ")").c_str());
        }
        while (true) {
            if (rect.origin.x >= texture->getPixelsWide()) break;

            frames.pushBack(SpriteFrame::createWithTexture(texture, rect));
            rect.origin.x += rect.size.width;
        }
    }
};