#ifndef _COMPONENT_PLAYER_MOVE_H_
#define _COMPONENT_PLAYER_MOVE_H_

#include "entities/units/components/ComponentBase.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

class PlayerMoveComponent : public ComponentBase {

public:
    PlayerMoveComponent(Unit* _ref) { ref = _ref; }

    bool facing_right = false;
    float move_vel_x = 200.0f;
    float jump_vel_x = 450.0f;
    const float LARGE_VEL_X = 100000.0f;
    bool jumping = false;
    bool moving = false;
    BulletAimerComponentPtr bullet_aimer;
    ColliderComponentPtr collider;

    void init();
    virtual bool on_contact_run(cc::PhysicsContact& contact);
    virtual void update();
    virtual void cleanup();
};

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS

#endif