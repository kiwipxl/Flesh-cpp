#ifndef _CRATE_H_
#define _CRATE_H_

#include <vector>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

#include "entities/EntityDefines.h"
#include "entities/EntityScheduler.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS
BEGIN_CRATES_NS

class Crate : public EntityScheduler {

public:
    Crate(int _x, int _y);

    cc::Sprite* base;
    cc::PhysicsBody* pbody;

    void update();
    bool on_contact_run(cc::PhysicsContact& contact);
};

extern std::vector<Crate*> crates;

extern void init();
extern void deinit();
extern void spawn(int _x, int _y);
extern void update();

END_CRATES_NS
END_ENTITIES_NS

#endif