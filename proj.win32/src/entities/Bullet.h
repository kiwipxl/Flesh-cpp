#ifndef _BULLET_H_
#define _BULLET_H_

/*
handles management and logic of bullets
*/

#include <vector>
#include <memory>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"
#include "physics/Physics.h"

#define BEGIN_BULLET_NS     namespace bullet {
#define END_BULLET_NS       };

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

namespace cc = cocos2d;

struct BulletLogicBase;

enum BulletType {

    BULLET_TYPE_DECAY, 
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
        bool on_contact_run(cc::PhysicsContact& contact);

        void add_logic_decay(float decay_after_seconds);
        void add_logic_test(float angle, float power);
        void add_logic_test2(float angle, float power);

        void schedule_removal();
        bool is_removal_scheduled() { return to_be_removed; }
        BulletType get_type() { return type; }

    protected:
        bool to_be_removed = false;
        BulletType type;
        std::vector<BulletLogicBase*> logic_list;
};

typedef std::shared_ptr<Bullet> BulletPtr;

extern void init();
extern void deinit();
extern BulletPtr create_bullet(int x, int y);
extern void update();

END_BULLET_NS
END_ENTITIES_NS

#endif