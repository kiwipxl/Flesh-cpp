#ifndef _BULLET_H_
#define _BULLET_H_

/*
handles management and logic of bullets
*/

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

    BULLET_TYPE_TEST, 
    BULLET_TYPE_TEST2
};

class Bullet {

    public:
        Bullet(int x, int y);
        ~Bullet();

        cc::Sprite* base;
        cc::PhysicsBody* pbody;

        void update();
        void cleanup();
        bool physics_contact(cc::PhysicsContact& contact);

        void add_btype_test(float angle, float power);
        void add_btype_test2(float angle, float power);

        void schedule_removal();
        bool is_removal_scheduled() { return to_be_removed; }
        BulletType get_type() { return type; }

    private:
        bool to_be_removed = false;
        BulletType type;
        std::function<void()> type_callback = nullptr;
};

typedef std::shared_ptr<Bullet> BulletPtr;

extern void init();
extern void deinit();
extern BulletPtr create_bullet(int x, int y);
extern void update();
extern bool physics_contact(cc::PhysicsContact& contact);

END_BULLET_NS
END_ENTITIES_NS

#endif