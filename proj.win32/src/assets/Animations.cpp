#include "Animations.h"

#include <renderer/CCTextureCache.h>

#include "utility/Logger.h"
#include "utility/String.h"

BEGIN_ASSETS_NS

namespace animations {

    using namespace cocos2d;

    void load_ani_frames(std::string, Vector<SpriteFrame*>&, Rect);

    Vector<SpriteFrame*> spinner_frames;

    void init() {
        load_ani_frames("ui/spinner_sheet.png", spinner_frames, Rect(0, 0, 34, 34));

        log_print_file << "animation assets loaded";
    }

    void load_ani_frames(std::string file_name, Vector<SpriteFrame*>& frames, Rect rect) {
        Texture2D* texture = TextureCache::sharedTextureCache()->addImage(file_name);

        //conditional asserts
        cf_assert(texture == NULL,
            sstream_cstr("could not load texture (" << file_name << ")"));

        cf_assert(texture->getPixelsWide() == 0 || texture->getPixelsHigh() == 0,
            sstream_cstr("texture width/height must be greater than 0 (" << file_name << ")"));

        cf_assert(rect.origin.x < 0 || rect.origin.y < 0,
            sstream_cstr("rect x/y cannot be less than 0 (" << file_name << ")"));

        cf_assert(rect.size.width < 0 || rect.size.height < 0,
            sstream_cstr("rect width/height cannot be less than 0 (" << file_name << ")"));

        cf_assert(rect.origin.x > texture->getPixelsWide() || rect.origin.y > texture->getPixelsHigh(),
            sstream_cstr("rect x/y cannot be greater than texture width/height (" << file_name << ")"));

        cf_assert(rect.size.width > texture->getPixelsWide() || rect.size.height > texture->getPixelsHigh(),
            sstream_cstr("rect width/height cannot be greater than texture width/height (" << file_name << ")"));

        while (true) {
            if (rect.origin.x >= texture->getPixelsWide()) break;

            frames.pushBack(SpriteFrame::createWithTexture(texture, rect));
            rect.origin.x += rect.size.width;
        }
        log_print_file << "loaded " << frames.size() << " frames (" << file_name << ")";
    }
};

END_ASSETS_NS