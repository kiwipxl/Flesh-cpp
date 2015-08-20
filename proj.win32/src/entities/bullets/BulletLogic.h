#ifndef _BULLET_LOGIC_H_
#define _BULLET_LOGIC_H_

/*
contains bullet logic components that inherit from a base logic class
logic components can be added or removed to a bullet object
*/

#include <vector>

#include "assets/Particles.h"
#include "debug/Logger.h"
#include "entities/EntityScheduler.h"
#include "entities/bullets/Bullet.h"
#include "entities/bullets/BulletGroup.h"
#include "entities/EntityDefines.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "entities/items/Item.h"
#include "states/Game.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS

namespace cc = cocos2d;

enum BulletLogicType {

    BULLET_LOGIC_DECAY,
    BULLET_LOGIC_TERRAIN_DESTROY,
    BULLET_LOGIC_FIRE_BULLET, 
    BULLET_LOGIC_MINI_FIRE_BULLET, 
    BULLET_LOGIC_C4
};

class BulletLogicBase : public EntityScheduler {

public:
    Bullet* ref;
    int timer = 0;
    const float max_vel = 1200.0f;

    BulletLogicBase(Bullet& bullet_ref) {
        ref = &bullet_ref;
    }

    virtual ~BulletLogicBase() { }
    virtual void cleanup() { }

    virtual void update() {
        update_scheduler();
    }

    virtual bool on_contact_run(cc::PhysicsContact& contact) {
        //checks if the bullet is colliding with the parent

        if (!bullet_left_parent) return false;

        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        if (a && b && CHECK_AB_COLLIDE(ref->base) && CHECK_AB_COLLIDE(ref->parent->unit_parent->base)) {
            bullet_left_parent = false;
        }

        return false;
    }
    virtual void on_contact_leave(cc::PhysicsContact& contact) {
        //checks if the bullet has left the parent

        if (bullet_left_parent) return;

        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        if (a && b && CHECK_AB_COLLIDE(ref->base) && CHECK_AB_COLLIDE(ref->parent->unit_parent->base)) {
            bullet_left_parent = true;
        }
    }

    void create_physics_body_box(int width, int height, cc::PhysicsMaterial* mat = NULL) {
        if (ref->pbody != NULL) { log_error << "tried to create physics body box, but one already existed"; ref->pbody->release(); }

        ref->pbody = cc::PhysicsBody::createBox(cc::Size(width, height), mat ? *mat : cc::PHYSICSBODY_MATERIAL_DEFAULT);
        ref->pbody->setCollisionBitmask(1);
        ref->pbody->setContactTestBitmask(true);
        ref->pbody->setRotationEnable(false);
        ref->pbody->setMass(100.0f);
        ref->pbody->setVelocityLimit(max_vel);
        ref->base->setPhysicsBody(ref->pbody);
    }

    bool has_bullet_left_parent() { return bullet_left_parent; }

private:
    bool bullet_left_parent = true;
};

class BulletLogicDecay : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_DECAY;
    clock_t start_time;
    float decay_after_seconds;

    BulletLogicDecay(Bullet& bullet_ref, float _decay_after_ms) : BulletLogicBase(bullet_ref) {
        decay_after_seconds = _decay_after_ms;
        start_time = clock();
    }

    virtual void update() {
        if (clock() - start_time >= decay_after_seconds) {
            ref->schedule_removal();
        }
    }
};

class BulletLogicTerrainDestroy : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_TERRAIN_DESTROY;

    BulletLogicTerrainDestroy(Bullet& bullet_ref) : BulletLogicBase(bullet_ref) {

    }

    virtual bool on_contact_run(cc::PhysicsContact& contact) {
        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        map::terrain::Terrain* t;
        if (a && b && CHECK_AB_COLLIDE(ref->base)) {
            if (t = states::game::terrain->is_terrain(a, b)) {
                //t->test_explosion_at(ref->base->getPosition());

                ref->schedule_removal();
            }
        }

        return false;
    }
};

class BulletLogicFireBullet : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_FIRE_BULLET;
    cc::ParticleSystemQuad* fire_trail_particle;
    const float DAMAGE = 4.0f;
    const float EXPLODE_AFTER_TIME = 600.0f;
    clock_t start_time;
    bool has_gen_explosion = false;

    BulletLogicFireBullet(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        create_physics_body_box(50.0f, 50.0f);
        ref->add_logic_decay(4000.0f + ((rand() / (float)RAND_MAX) * 4000.0f));

        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        ref->pbody->applyImpulse(cc::Vec2(force_x, force_y));
        ref->add_logic_terrain_destroy();
        
        fire_trail_particle = cc::ParticleSystemQuad::create(assets::particles::bullet_fire_trail);
        fire_trail_particle->setPosition(ref->base->getPosition());
        fire_trail_particle->setScale(.65f);
        root::map_layer->addChild(fire_trail_particle, 1);

        start_time = clock();
    }

    void gen_explosion() {
        if (has_gen_explosion) return;
        
        for (int n = 0; n < 14; ++n) {
            auto b = ref->parent->create_bullet(ref->base->getPositionX(), ref->base->getPositionY() + 40);
            float angle = ((rand() / (float)RAND_MAX) * 50.0f) + 65.0f;
            b->add_logic_mini_fire_bullet(angle, ((rand() / (float)RAND_MAX) * .2f) + .5f);
        }
        auto bullet_explosion = cc::ParticleSystemQuad::create(assets::particles::ring);
        bullet_explosion->setPosition(ref->base->getPosition());
        bullet_explosion->setScale(.8f);
        bullet_explosion->setAutoRemoveOnFinish(true);
        root::map_layer->addChild(bullet_explosion, 1);
        ref->schedule_removal();

        has_gen_explosion = true;
    }

    virtual ~BulletLogicFireBullet() {
        root::map_layer->removeChild(fire_trail_particle, 1);

        auto bullet_explosion = cc::ParticleSystemQuad::create(assets::particles::bullet_explosion);
        bullet_explosion->setPosition(ref->base->getPosition());
        bullet_explosion->setScale(.325f);
        root::map_layer->addChild(bullet_explosion, 1);
        bullet_explosion->setAutoRemoveOnFinish(true);
    }

    virtual void cleanup() {
        gen_explosion();
    }

    virtual void update() {
        fire_trail_particle->setPosition(ref->base->getPosition());

        if (clock() - start_time >= EXPLODE_AFTER_TIME) {
            gen_explosion();
        }
    }

    virtual bool on_contact_run(cc::PhysicsContact& contact) {
        BulletLogicBase::on_contact_run(contact);

        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        if (a && b && has_bullet_left_parent() && CHECK_AB_COLLIDE(ref->base)) {
            for (auto& u : units::all_units) {
                if (CHECK_AB_COLLIDE(u->base)) {
                    u->take_damage(DAMAGE);
                    ref->schedule_removal();

                    return true;
                }
            }
        }

        return false;
    }
};

class BulletLogicMiniFireBullet : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_MINI_FIRE_BULLET;
    cc::ParticleSystemQuad* fire_trail_particle;
    const float DAMAGE = 2.5f;

    BulletLogicMiniFireBullet(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        create_physics_body_box(32.0f, 32.0f);
        ref->add_logic_decay(4000.0f + ((rand() / (float)RAND_MAX) * 4000.0f));

        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        ref->pbody->applyImpulse(cc::Vec2(force_x, force_y));

        ref->add_logic_terrain_destroy();

        fire_trail_particle = cc::ParticleSystemQuad::create(assets::particles::bullet_fire_trail);
        fire_trail_particle->setPosition(ref->base->getPosition());
        fire_trail_particle->setScale(.25f);
        root::map_layer->addChild(fire_trail_particle, 1);
    }

    virtual ~BulletLogicMiniFireBullet() {
        root::map_layer->removeChild(fire_trail_particle, 1);

        auto bullet_explosion = cc::ParticleSystemQuad::create(assets::particles::bullet_explosion);
        bullet_explosion->setPosition(ref->base->getPosition());
        bullet_explosion->setScale(.325f);
        root::map_layer->addChild(bullet_explosion, 1);
        bullet_explosion->setAutoRemoveOnFinish(true);
    }

    virtual void update() {
        fire_trail_particle->setPosition(ref->base->getPosition());
    }

    virtual bool on_contact_run(cc::PhysicsContact& contact) {
        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        if (a && b && CHECK_AB_COLLIDE(ref->base)) {
            for (auto& u : units::all_units) {
                if (CHECK_AB_COLLIDE(u->base)) {
                    u->take_damage(DAMAGE);
                    ref->schedule_removal();
                    return true;
                }
            }
            for (auto& i : items::item_list) {
                if (CHECK_AB_COLLIDE(i->base)) {
                    ref->schedule_removal();
                    return true;
                }
            }
        }

        return false;
    }
};

class BulletLogicC4 : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_C4;
    const float MAX_DAMAGE = 8.5f;
    const float EXPLODE_AFTER_TIME = 5000.0f;
    clock_t start_time;
    bool has_gen_explosion = false;

    BulletLogicC4(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        ref->base->setTexture(assets::textures::c4);
        cc::Size s = ref->base->getTexture()->getContentSize();
        ref->base->setTextureRect(cc::Rect(0, 0, s.width, s.height));
        ref->base->setScale(.55f);

        cc::PhysicsMaterial mat;
        mat.density = .1f;
        mat.friction = .4f;
        mat.restitution = 1.2f;
        create_physics_body_box(64.0f, 64.0f, &mat);
        ref->pbody->setRotationEnable(true);
        ref->base->setRotation(angle);

        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        ref->pbody->applyImpulse(cc::Vec2(force_x, force_y));
        ref->pbody->applyTorque(200.0f);

        start_time = clock();
    }

    virtual ~BulletLogicC4() {

    }

    virtual void update() {
        if (has_gen_explosion) return;

        //if time is over or the time is halfway through and the bullet is not moving
        if (clock() - start_time >= EXPLODE_AFTER_TIME || 
           (ref->pbody->getVelocity().x >= -2.0f && ref->pbody->getVelocity().x <= 2.0f && 
            ref->pbody->getVelocity().y >= -2.0f && ref->pbody->getVelocity().y <= 2.0f &&
            clock() - start_time >= EXPLODE_AFTER_TIME / 2.0f)) {

            has_gen_explosion = true;
            ref->base->setVisible(false);
            ref->base->setPhysicsBody(NULL);

            auto bullet_explosion = cc::ParticleSystemQuad::create("c4_explosion.plist");
            bullet_explosion->setRadialAccel(-1000.0f);
            bullet_explosion->setSpeed(220.0f);
            bullet_explosion->setDuration(.25f);
            bullet_explosion->setPosition(ref->base->getPosition());
            bullet_explosion->setScale(3.0f);
            root::map_layer->addChild(bullet_explosion, 1);
            bullet_explosion->setAutoRemoveOnFinish(true);

            ref->schedule_removal_in(1500.0f);

            cc::Vec2 center = ref->base->getPosition();
            for (auto& u : units::all_units) {
                float dist = sqrt(pow(u->base->getPositionX() - center.x, 2) + pow(u->base->getPositionY() - center.y, 2));
                float radius = 250;
                float angle = atan2(u->base->getPositionY() - center.y, u->base->getPositionX() - center.x);
                float force_x = cos(angle) * MAX(radius - dist, 0);
                float force_y = sin(angle) * MAX(radius - dist, 0);
                u->pbody->setVelocity(cc::Vec2(MIN(force_x * 55.0f, max_vel), MIN(abs(force_x) * 200.0f, max_vel * 8.0f)));
                u->take_damage((cc::clampf(radius - dist, 0, radius) / radius) * MAX_DAMAGE);
            }
        }
    }

    virtual bool on_contact_run(cc::PhysicsContact& contact) {
        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        map::terrain::Terrain* t;
        if (a && b && CHECK_AB_COLLIDE(ref->base)) {
            if (t = states::game::terrain->is_terrain(a, b)) {
                return true;
            }
        }

        return false;
    }
};

END_BULLETS_NS
END_ENTITIES_NS

#endif