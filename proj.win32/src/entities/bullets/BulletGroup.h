#ifndef _BULLET_GROUP_H_
#define _BULLET_GROUP_H_

/*
handles management of groups of bullets
*/

#include <vector>
#include <memory>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"
#include "physics/Physics.h"

BEGIN_ENTITIES_NS
BEGIN_BULLETS_NS

namespace cc = cocos2d;

class BulletGroup {

public:
    BulletGroup(units::Unit* _unit_parent);
    ~BulletGroup();

    std::vector<BulletPtr> bullets;
    units::Unit* unit_parent = NULL;
    BulletGroupPtr self_ptr;

    cc::Vec2 min_pos;
    cc::Vec2 max_pos;
    cc::Size max_size;

    BulletPtr create_bullet(int _x, int _y);

    void update();

    void schedule_removal() { removal_scheduled = true; }
    bool is_removal_scheduled() { return removal_scheduled; }

protected:
    bool removal_scheduled = false;
};

extern void init();
extern void deinit();
extern BulletGroupPtr create_group(units::Unit* _unit_parent);
extern void update();

END_BULLETS_NS
END_ENTITIES_NS

#endif