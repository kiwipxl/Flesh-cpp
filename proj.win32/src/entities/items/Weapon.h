#ifndef _WEAPON_H_
#define _WEAPON_H_

/*
weapon class that stores weapon attributes
*/

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
    Weapon(const char* _name, cc::Texture2D* _texture, float _scale, float _rotation_offset, bool _weapon_flippable);

    int get_id() { return id; }
    const char* get_name() { return name; }
    cc::Texture2D* get_texture() { return texture; }
    float get_scale() { return scale; }
    float get_rotation_offset() { return rotation_offset; }
    bool is_weapon_flippable() { return weapon_flippable; }

private:
    int id;
    const char* name;
    cc::Texture2D* texture;
    float scale;
    float rotation_offset;
    bool weapon_flippable;

};

extern Weapon* weapon_none;
extern Weapon* weapon_flame_fireworks;
extern Weapon* weapon_c4;

extern void init_weapons();
extern Weapon* get_weapon(int index);

END_ITEMS_NS
END_ENTITIES_NS

#endif