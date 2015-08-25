#include "entities/bullets/BulletGroup.h"

#include "assets/Textures.h"
#include "entities/bullets/Bullet.h"
#include "entities/bullets/BulletLogic.h"
#include "entities/units/Unit.h"
#include "SceneManager.h"
#include "StateManager.h"
#include "utility/Logger.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS

//private
std::vector<BulletGroupPtr> bullet_groups;

bool on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        bool do_collide = false;
        for (int i = 0; i < bullet_groups.size(); ++i) {
            for (int n = 0; n < bullet_groups[i]->bullets.size(); ++n) {
                if (bullet_groups[i]->bullets[n]->on_contact_run(contact)) do_collide = true;
            }
        }
        return do_collide;
    }

    return false;
}

void on_contact_leave(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        for (auto& bg : bullet_groups) {
            for (auto& b : bg->bullets) {
                if (!b) continue;

                if (!b->is_removal_scheduled()) {
                    b->on_contact_leave(contact);
                }
            }
        }
    }
}

//public
void init() {
    physics::add_on_contact_run(on_contact_run, NULL);
    physics::add_on_contact_leave(on_contact_leave, NULL);
}

void deinit() {
    physics::remove_on_contact_run(on_contact_run);
    physics::remove_on_contact_leave(on_contact_leave);
}

BulletGroupPtr create_group(units::Unit* _unit_parent) {
    BulletGroupPtr b(new BulletGroup(_unit_parent));
    b->self_ptr = b;
    bullet_groups.push_back(b);
    return b;
}

void update() {
    for (int n = 0; n < bullet_groups.size(); ++n) {
        auto& b = bullet_groups[n];
        if (!b) continue;
        if (!b->is_removal_scheduled()) b->update();
        if (b->is_removal_scheduled()) {
            bullet_groups.erase(bullet_groups.begin() + n, bullet_groups.begin() + n + 1);
            --n;
        }
    }
}

//-- begin BulletGroup class --

BulletGroup::BulletGroup(units::Unit* _unit_parent) {
    unit_parent = _unit_parent;
}

BulletGroup::~BulletGroup() {
    bullet_groups.clear();
}

BulletPtr BulletGroup::create_bullet(int _x, int _y) {
    BulletPtr b(new Bullet(_x, _y, self_ptr));
    bullets.push_back(b);
    return b;
}

void BulletGroup::update() {
    min_pos = Vec2(FLT_MAX, FLT_MAX);
    max_pos = Vec2(-FLT_MAX, -FLT_MAX);

    for (int n = 0; n < bullets.size(); ++n) {
        if (!bullets[n]) continue;
        if (!bullets[n]->is_removal_scheduled()) bullets[n]->update();
        if (bullets[n]->is_removal_scheduled()) {
            for (auto& l : bullets[n]->logic_list) {
                l->cleanup();
            }
            bullets.erase(bullets.begin() + n, bullets.begin() + n + 1);
            --n;
        }
    }

    max_size.width = max_pos.x - min_pos.x;
    max_size.height = max_pos.y - min_pos.y;

    if (bullets.size() == 0) schedule_removal();
}

//-- end BulletGroup class --

END_BULLETS_NS
END_ENTITIES_NS