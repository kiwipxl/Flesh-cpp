#ifndef _COMPONENT_COLLIDER_H_
#define _COMPONENT_COLLIDER_H_

#include "entities/units/components/ComponentBase.h"

namespace cc = cocos2d;
namespace cocos2d {

    class PhysicsContact;
};

BEGIN_ENTITIES_NS

class Unit;

BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

struct BulletAimerComponent;

class ColliderComponent : public ComponentBase {

public:
    ColliderComponent(Unit* _ref) { ref = _ref; }

    bool colliding = false;                     //test whether colliding or not (no delay)
    bool colliding_with_delay = false;          //includes a delay for when colliding switches off so it's not immediate

    void init();
    virtual bool on_contact_run(cc::PhysicsContact& contact);
    virtual void update();

private:
    int collide_timer = 0;
};

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS

#endif