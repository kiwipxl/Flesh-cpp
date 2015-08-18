#ifndef _COMPONENT_BASE_H_
#define _COMPONENT_BASE_H_

#include "entities/EntityDefines.h"

#define BEGIN_COMPONENTS_NS     namespace components {
#define END_COMPONENTS_NS       };

namespace cc = cocos2d;

namespace cocos2d {

    class PhysicsContact;
};

BEGIN_ENTITIES_NS

class Unit;

BEGIN_COMPONENTS_NS

class ComponentBase {

public:
    Unit* ref;

    virtual void update() = 0;
    virtual bool on_contact_run(cc::PhysicsContact& contact) { return false; }
};

END_COMPONENTS_NS
END_ENTITIES_NS

#endif