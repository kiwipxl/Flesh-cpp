#ifndef _COMPONENT_BASE_H_
#define _COMPONENT_BASE_H_

#include <physics/CCPhysicsContact.h>

#include "entities/EntityDefines.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS

//forward declare unit in entities::units namespace
namespace units {

    class Unit;
}

BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

class ComponentBase {

public:
    virtual ~ComponentBase() { }

    Unit* ref;

    virtual void update() = 0;
    virtual bool on_contact_run(cc::PhysicsContact& contact) { return false; }

    void schedule_removal() { scheduled_removal = true; }
    bool is_removal_scheduled() { return scheduled_removal; }

private:
    bool scheduled_removal = false;
};

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS

#endif