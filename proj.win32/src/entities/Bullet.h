#ifndef _BULLET_H_
#define _BULLET_H_

#include <vector>
#include <memory>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsContact.h>

#include "entities/EntityDefines.h"

#define BEGIN_BULLET_NS     namespace bullet {
#define END_BULLET_NS       };

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

namespace cc = cocos2d;

enum BulletType {

    TEST_BULLET
};

class Bullet {

    public:
        Bullet(BulletType type, int x, int y);
        ~Bullet();

        cc::Sprite* base;
        cc::PhysicsBody* pbody;

        void update();
        void cleanup();
        bool physics_contact(cc::PhysicsContact& contact);

        bool is_removal_scheduled() { return to_be_removed; }

    private:
        cc::EventListenerPhysicsContact* pcontact_listener;
        bool to_be_removed = false;

        void schedule_removal();
};

typedef std::shared_ptr<Bullet> BulletPtr;

extern BulletPtr create_bullet(BulletType type, int x, int y);
extern void update();

END_BULLET_NS
END_ENTITIES_NS

#endif