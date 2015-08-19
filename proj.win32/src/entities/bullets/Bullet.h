#ifndef _BULLET_H_
#define _BULLET_H_

/*
handles individual bullets and management of their logic components
*/

#include <vector>
#include <memory>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"
#include "physics/Physics.h"

BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS

namespace cc = cocos2d;

struct BulletLogicBase;

enum BulletLogicType;

class Bullet {

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

        void schedule_removal() { removal_scheduled = true; }
        void schedule_removal_in(float ms) { removal_start_time = clock(); removal_in_ms = ms; }
        bool is_removal_scheduled() { return removal_scheduled; }

    protected:
        bool removal_scheduled = false;
        clock_t removal_start_time;
        float removal_in_ms = 0.0f;
        std::vector<BulletLogicBase*> logic_list;
};

END_BULLETS_NS
END_ENTITIES_NS

#endif