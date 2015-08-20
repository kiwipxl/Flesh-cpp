#ifndef _WEAPON_H_
#define _WEAPON_H_

#include "entities/EntityDefines.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS
BEGIN_ITEMS_NS

enum WeaponID {

    WEAPON_ID_FLAME_FIREWORKS,
    WEAPON_ID_C4
};

class Weapon {

public:
    Weapon(const char* _name, cc::Texture2D* _texture, float _scale, float _rotation_offset);

private:
    int id;
    const char* name;
    cc::Texture2D* texture;
    float scale;
    float rotation_offset;
};

extern Weapon* weapon_flame_fireworks;
extern Weapon* weapon_c4;

END_ITEMS_NS
END_ENTITIES_NS

#endif