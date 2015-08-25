#include "entities/bullets/Bullet.h"

#include "assets/Textures.h"
#include "entities/bullets/BulletLogic.h"
#include "entities/units/Unit.h"
#include "SceneManager.h"
#include "StateManager.h"
#include "utility/Logger.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS

//private

//-- begin Bullet class --

Bullet::Bullet(int x, int y, BulletGroupPtr& _parent) {
    parent = _parent;

    base = Sprite::create();
    base->setPosition(x, y);
    root::map_layer->addChild(base, 1);
}

Bullet::~Bullet() {
    for (int n = 0; n < logic_list.size(); ++n) {
        delete logic_list[n];
    }
    logic_list.clear();
    root::map_layer->removeChild(base, 1);
}

bool Bullet::on_contact_run(PhysicsContact& contact) {
    bool do_collide = false;
    for (int n = 0; n < logic_list.size(); ++n) {
        if (logic_list[n]->on_contact_run(contact)) do_collide = true;
    }
    return do_collide;
}

void Bullet::on_contact_leave(PhysicsContact& contact) {
    for (int n = 0; n < logic_list.size(); ++n) {
        logic_list[n]->on_contact_leave(contact);
    }
}

void Bullet::update() {
    update_scheduler();

    parent->min_pos.x = MIN(base->getPositionX(), parent->min_pos.x);
    parent->min_pos.y = MIN(base->getPositionY(), parent->min_pos.y);
    parent->max_pos.x = MAX(base->getPositionX(), parent->max_pos.x);
    parent->max_pos.y = MAX(base->getPositionY(), parent->max_pos.y);

    for (int n = 0; n < logic_list.size(); ++n) {
        auto& l = logic_list[n];
        if (!l) continue;
        if (!l->is_removal_scheduled()) l->update();
        if (l->is_removal_scheduled()) {
            l->cleanup();
            logic_list.erase(logic_list.begin() + n, logic_list.begin() + n + 1);
            --n;
        }
    }
}

void Bullet::add_logic_decay(float decay_after_ms) {
    logic_list.push_back(new BulletLogicDecay(*this, decay_after_ms));
}

void Bullet::add_logic_terrain_destroy() {
    logic_list.push_back(new BulletLogicTerrainDestroy(*this));
}

void Bullet::add_logic_fire_bullet(float angle, float power) {
    logic_list.push_back(new BulletLogicFireBullet(*this, angle, power));
}

void Bullet::add_logic_mini_fire_bullet(float angle, float power) {
    logic_list.push_back(new BulletLogicMiniFireBullet(*this, angle, power));
}

void Bullet::add_logic_c4(float angle, float power) {
    logic_list.push_back(new BulletLogicC4(*this, angle, power));
}

//-- end Bullet class --

END_BULLETS_NS
END_ENTITIES_NS