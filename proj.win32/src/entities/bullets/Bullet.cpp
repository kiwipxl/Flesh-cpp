#include "entities/bullets/Bullet.h"

#include "assets/Textures.h"
#include "entities/bullets/BulletLogic.h"
#include "entities/units/Unit.h"
#include "debug/Logger.h"
#include "SceneManager.h"
#include "StateManager.h"

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

    add_logic_decay(4.0f + ((rand() / (float)RAND_MAX) * 4.0f));
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
    parent->min_pos.x = MIN(base->getPositionX(), parent->min_pos.x);
    parent->min_pos.y = MIN(base->getPositionY(), parent->min_pos.y);
    parent->max_pos.x = MAX(base->getPositionX(), parent->max_pos.x);
    parent->max_pos.y = MAX(base->getPositionY(), parent->max_pos.y);

    for (int n = 0; n < logic_list.size(); ++n) {
        logic_list[n]->update();
    }
}

void Bullet::add_logic_decay(float decay_after_seconds) {
    logic_list.push_back(new BulletLogicDecay(*this, decay_after_seconds));
}

void Bullet::add_logic_terrain_destroy() {
    logic_list.push_back(new BulletLogicTerrainDestroy(*this));
}

void Bullet::add_logic_test(float angle, float power) {
    logic_list.push_back(new BulletLogicTest(*this, angle, power));
}

void Bullet::add_logic_test2(float angle, float power) {
    logic_list.push_back(new BulletLogicTest2(*this, angle, power));
}

//-- end Bullet class --

END_BULLETS_NS
END_ENTITIES_NS