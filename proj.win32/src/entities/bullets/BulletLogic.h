#ifndef _BULLET_LOGIC_H_
#define _BULLET_LOGIC_H_

/*
contains bullet logic components that inherit from a base logic class
logic components can be added or removed to a bullet object
*/

#include <vector>

#include "assets/Particles.h"
#include "debug/Logger.h"
#include "entities/bullets/Bullet.h"
#include "entities/EntityDefines.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "states/Game.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

namespace cc = cocos2d;

enum BulletLogicType {

    BULLET_LOGIC_DECAY,
    BULLET_LOGIC_TERRAIN_DESTROY,
    BULLET_LOGIC_TEST,
    BULLET_LOGIC_TEST2
};

class BulletLogicBase {

public:
    Bullet* ref;
    int timer = 0;

    BulletLogicBase(Bullet& bullet_ref) {
        ref = &bullet_ref;
    }

    virtual ~BulletLogicBase() { }

    virtual void update() { }
    virtual bool on_contact_run(cc::PhysicsContact& contact) {
        //checks if the bullet is colliding with the parent

        if (!bullet_left_parent) return false;

        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        if (a && b && CHECK_AB_COLLIDE(ref->base) && CHECK_AB_COLLIDE(ref->unit_parent->base)) {
            bullet_left_parent = false;
        }

        return false;
    }
    virtual void on_contact_leave(cc::PhysicsContact& contact) {
        //checks if the bullet has left the parent

        if (bullet_left_parent) return;

        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();

        if (a && b && CHECK_AB_COLLIDE(ref->base) && CHECK_AB_COLLIDE(ref->unit_parent->base)) {
            bullet_left_parent = true;
        }
    }

    void create_physics_body_box(int width, int height) {
        if (ref->pbody != NULL) { log_error << "tried to create physics body box, but one already existed"; ref->pbody->release(); }

        ref->pbody = cc::PhysicsBody::createBox(cc::Size(width, height));
        ref->pbody->setCollisionBitmask(1);
        ref->pbody->setContactTestBitmask(true);
        ref->pbody->setRotationEnable(false);
        ref->pbody->setMass(100.0f);
        ref->base->setPhysicsBody(ref->pbody);
    }

    bool has_bullet_left_parent() { return bullet_left_parent; }

private:
    bool bullet_left_parent = true;
};

class BulletLogicDecay : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_DECAY;
    time_t start_time = 0;
    float decay_after_seconds;

    BulletLogicDecay(Bullet& bullet_ref, float _decay_after_seconds) : BulletLogicBase(bullet_ref) {
        decay_after_seconds = _decay_after_seconds;
    }

    virtual void update() {
        if (start_time == 0) time(&start_time);

        time_t temp_time;
        if (time(&temp_time) - start_time >= decay_after_seconds) {
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

class BulletLogicTest : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_TEST;
    bool gen_explosion = false;
    cc::ParticleSystemQuad* fire_trail_particle;
    const float DAMAGE = 4.0f;

    BulletLogicTest(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        create_physics_body_box(32.0f, 32.0f);

        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        ref->pbody->applyImpulse(cc::Vec2(force_x, force_y));
        ref->add_logic_terrain_destroy();
        
        fire_trail_particle = cc::ParticleSystemQuad::create(assets::particles::bullet_fire_trail);
        fire_trail_particle->setPosition(ref->base->getPosition());
        fire_trail_particle->setScale(.65f);
        root::map_layer->addChild(fire_trail_particle, 1);
    }

    virtual ~BulletLogicTest() {
        root::map_layer->removeChild(fire_trail_particle, 1);

        auto bullet_explosion = cc::ParticleSystemQuad::create(assets::particles::bullet_explosion_name);
        bullet_explosion->setPosition(ref->base->getPosition());
        bullet_explosion->setScale(.325f);
        root::map_layer->addChild(bullet_explosion, 1);
        bullet_explosion->setAutoRemoveOnFinish(true);
    }

    virtual void update() {
        fire_trail_particle->setPosition(ref->base->getPosition());

        if (gen_explosion) return;
        if (++timer >= 40) {
            gen_explosion = true;
            for (int n = 0; n < 14; ++n) {
                auto b = create_bullet(ref->base->getPositionX(), ref->base->getPositionY(), ref->unit_parent);
                float angle = ((rand() / (float)RAND_MAX) * 50.0f) + 65.0f;
                b->add_logic_test2(angle, ((rand() / (float)RAND_MAX) * .2f) + .5f);
            }
            auto bullet_explosion = cc::ParticleSystemQuad::create("Ring.plist");
            bullet_explosion->setPosition(ref->base->getPosition());
            bullet_explosion->setScale(.8f);
            bullet_explosion->setAutoRemoveOnFinish(true);
            root::map_layer->addChild(bullet_explosion, 1);
            ref->schedule_removal();
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
                }
            }
        }

        return false;
    }
};

class BulletLogicTest2 : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_TEST2;
    bool gen_explosion = false;
    cc::ParticleSystemQuad* fire_trail_particle;
    const float DAMAGE = 2.5f;

    BulletLogicTest2(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        create_physics_body_box(20.0f, 20.0f);

        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        ref->pbody->applyImpulse(cc::Vec2(force_x, force_y));

        ref->base->setTexture(assets::textures::test_bullet);
        ref->add_logic_terrain_destroy();

        fire_trail_particle = cc::ParticleSystemQuad::create(assets::particles::bullet_fire_trail);
        fire_trail_particle->setPosition(ref->base->getPosition());
        fire_trail_particle->setScale(.25f);
        root::map_layer->addChild(fire_trail_particle, 1);
    }

    virtual ~BulletLogicTest2() {
        root::map_layer->removeChild(fire_trail_particle, 1);

        auto bullet_explosion = cc::ParticleSystemQuad::create(assets::particles::bullet_explosion_name);
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
                }
            }
        }

        return false;
    }
};

END_BULLET_NS
END_ENTITIES_NS

#endif