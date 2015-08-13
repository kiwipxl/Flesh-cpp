#ifndef _BULLET_H_
#define _BULLET_H_

#include <vector>
#include <memory>

#include "entities/EntityDefines.h"

BEGIN_ENTITIES_NS

class Bullet {

    public:
        Bullet();
};

typedef std::unique_ptr<Bullet> BulletPtr;

extern BulletPtr create_bullet();

END_ENTITIES_NS

#endif