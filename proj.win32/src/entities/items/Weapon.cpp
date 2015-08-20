#include "Weapon.h"

#include <renderer/CCTexture2D.h>

#include "assets/Textures.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_ITEMS_NS

//private
int unique_id_count = 0;

//public
Weapon* weapon_flame_fireworks = new Weapon { "Flame Fireworks", assets::textures::laser_machine_gun, .2f, -180 };
Weapon* weapon_c4 = new Weapon { "C4", assets::textures::c4, .55f, 90 };

//-- begin Weapon class --

Weapon::Weapon(const char* _name, Texture2D* _texture, float _scale, float _rotation_offset) {
    name = _name;
    id = ++unique_id_count;
    texture = _texture;
    scale = _scale;
    rotation_offset = _rotation_offset;
}

//-- end Weapon class --

END_ITEMS_NS
END_ENTITIES_NS