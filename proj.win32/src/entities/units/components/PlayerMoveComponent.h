#ifndef _COMPONENT_PLAYER_MOVE_H_
#define _COMPONENT_PLAYER_MOVE_H_

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
struct ColliderComponent;

class PlayerMoveComponent : public ComponentBase {

public:
    PlayerMoveComponent(Unit* _ref) { ref = _ref; }

    bool facing_right = false;
    float move_vel_x = 200.0f;
    float jump_vel_x = 450.0f;
    const float LARGE_VEL_X = 100000.0f;
    bool jumping = false;
    bool moving = false;
    BulletAimerComponent* bullet_aimer;
    ColliderComponent* collider;

    void init();
    virtual bool on_contact_run(cc::PhysicsContact& contact);
    virtual void update();
};

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS

#endif