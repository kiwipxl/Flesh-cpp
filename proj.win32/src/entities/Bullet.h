#ifndef _BULLET_H_
#define _BULLET_H_

#include <vector>
#include <memory>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"

#define BEGIN_BULLET_NS     namespace bullet {
#define END_BULLET_NS       };


BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

namespace cc = cocos2d;

enum BulletType {

    TEST_BULLET
};

class Bullet {

    public:
        Bullet(BulletType type, int x, int y);
        ~Bullet();

        cc::Sprite* base;

        void update();
};

typedef std::unique_ptr<Bullet> BulletPtr;

extern BulletPtr create_bullet(BulletType type, int x, int y);
extern void update();

END_BULLET_NS
END_ENTITIES_NS

#endif