#ifndef _COMPONENT_BASE_H_
#define _COMPONENT_BASE_H_

#include <physics/CCPhysicsContact.h>

#include "debug/Logger.h"
#include "entities/EntityDefines.h"
#include "entities/EntityScheduler.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

enum ComponentType {

    UNIT_COMPONENT_TYPE_BULLET_AIMER, 
    UNIT_COMPONENT_TYPE_COLLIDER, 
    UNIT_COMPONENT_TYPE_PLAYER_MOVE
};

class ComponentBase : public EntityScheduler {

public:
    virtual ~ComponentBase() { }

    Unit* ref;
    ComponentType type;

    virtual void update() = 0;
    virtual bool on_contact_run(cc::PhysicsContact& contact) { return false; }
    virtual void cleanup() { }
};

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS

#endif