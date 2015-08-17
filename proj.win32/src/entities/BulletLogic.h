#ifndef _BULLET_LOGIC_H_
#define _BULLET_LOGIC_H_

#include <vector>

#include "assets/Particles.h"
#include "entities/Bullet.h"
#include "entities/EntityDefines.h"
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
    Bullet* bref;
    int timer = 0;

    BulletLogicBase(Bullet& bullet_ref) {
        bref = &bullet_ref;
    }

    virtual ~BulletLogicBase() {

    }

    virtual void update() { }
    virtual bool on_contact_run(cc::PhysicsContact& contact) { return false; }
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
            bref->schedule_removal();
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

        if (a && b && states::game::terrain->is_terrain(a, b)) {
            auto bullet_explosion = cc::ParticleSystemQuad::create(assets::particles::bullet_explosion_name);
            bullet_explosion->setPosition(bref->base->getPosition());
            bullet_explosion->setScale(.325f);
            root::map_layer->addChild(bullet_explosion, 1);
            bullet_explosion->setAutoRemoveOnFinish(true);

            bref->schedule_removal();
        }

        return false;
    }
};

class BulletLogicTest : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_TEST;
    bool gen_explosion = false;

    BulletLogicTest(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        bref->pbody->applyImpulse(cc::Vec2(force_x, force_y));
        bref->add_logic_terrain_destroy();
    }

    virtual ~BulletLogicTest() {

    }

    virtual void update() {
        if (gen_explosion) return;
        if (++timer >= 40) {
            gen_explosion = true;
            for (int n = 0; n < 8; ++n) {
                auto b = create_bullet(bref->base->getPositionX(), bref->base->getPositionY());
                float angle = ((rand() / (float)RAND_MAX) * 90.0f) + 45.0f;
                b->add_logic_test2(angle, ((rand() / (float)RAND_MAX) * .2f) + .5f);
            }
            auto bullet_explosion = cc::ParticleSystemQuad::create("Ring.plist");
            bullet_explosion->setPosition(bref->base->getPosition());
            bullet_explosion->setScale(.8f);
            bullet_explosion->setAutoRemoveOnFinish(true);
            root::map_layer->addChild(bullet_explosion, 1);
            bref->schedule_removal();
        }
    }
};

class BulletLogicTest2 : public BulletLogicBase {

public:
    const BulletLogicType logic_type = BULLET_LOGIC_TEST2;
    bool gen_explosion = false;

    BulletLogicTest2(Bullet& bullet_ref, float angle, float power) : BulletLogicBase(bullet_ref) {
        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        bref->pbody->applyImpulse(cc::Vec2(force_x, force_y));

        bref->add_logic_terrain_destroy();
    }
};

END_BULLET_NS
END_ENTITIES_NS

#endif