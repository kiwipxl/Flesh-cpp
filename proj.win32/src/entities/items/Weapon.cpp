#include "Weapon.h"

#include <renderer/CCTexture2D.h>

#include "assets/Textures.h"
#include "debug/Logger.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_ITEMS_NS

//private
int unique_id_count = 0;
std::vector<Weapon*> weapon_list;

//public
Weapon* weapon_none;
Weapon* weapon_flame_fireworks;
Weapon* weapon_c4;

void init_weapons() {
    weapon_none = new Weapon{ "empty", NULL, 0, 0, false };
    weapon_flame_fireworks = new Weapon{ "fireworks gun", assets::textures::laser_machine_gun, .2f, -180, true };
    weapon_c4 = new Weapon{ "C4", assets::textures::c4, .55f, 90, false };
}

Weapon* get_weapon(int index) {
    cf_assert(index < 0 || index >= weapon_list.size(), sstream_cstr("failed to get weapon, index " << index << " out of range"));
    return weapon_list[index + 1];
}

//-- begin Weapon class --

Weapon::Weapon(const char* _name, Texture2D* _texture, float _scale, float _rotation_offset, bool _weapon_flippable) {
    name = _name;
    id = ++unique_id_count;
    texture = _texture;
    scale = _scale;
    rotation_offset = _rotation_offset;
    weapon_flippable = _weapon_flippable;

    weapon_list.push_back(this);
}

//-- end Weapon class --

END_ITEMS_NS
END_ENTITIES_NS