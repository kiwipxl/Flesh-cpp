#ifndef _BULLET_H_
#define _BULLET_H_

/*
handles individual bullets and management of their logic components
*/

#include <vector>
#include <memory>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"
#include "entities/EntityScheduler.h"
#include "physics/Physics.h"

BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS

namespace cc = cocos2d;

struct BulletLogicBase;

enum BulletLogicType;

class Bullet : public EntityScheduler {

    public:
        Bullet(int x, int y, BulletGroupPtr& _parent);
        ~Bullet();

        cc::Sprite* base = NULL;
        cc::PhysicsBody* pbody = NULL;
        BulletGroupPtr parent;

        void update();
        bool on_contact_run(cc::PhysicsContact& contact);
        void on_contact_leave(cc::PhysicsContact& contact);

        void add_logic_decay(float decay_after_ms);
        void add_logic_terrain_destroy();
        void add_logic_fire_bullet(float angle, float power);
        void add_logic_mini_fire_bullet(float angle, float power);
        void add_logic_c4(float angle, float power);

        std::vector<BulletLogicBase*> logic_list;
};

END_BULLETS_NS
END_ENTITIES_NS

#endif