#ifndef _COMPONENT_PLAYER_MOVE_H_
#define _COMPONENT_PLAYER_MOVE_H_

#include "entities/components/ComponentBase.h"

namespace cc = cocos2d;
namespace cocos2d {

    class PhysicsContact;
};

BEGIN_ENTITIES_NS

class Unit;

BEGIN_COMPONENTS_NS

struct BulletAimerComponent;

class PlayerMoveComponent : public ComponentBase {

public:
    bool can_jump = false;
    bool facing_right = false;
    float move_vel_x = 200.0f;
    float jump_vel_x = 450.0f;
    const float LARGE_VEL_X = 100000.0f;
    bool jumping = false;
    bool colliding = false;
    int collide_timer = 0;
    bool moving = false;
    BulletAimerComponent* bullet_aimer;

    PlayerMoveComponent(Unit* _ref) { ref = _ref; }

    void init();
    virtual bool on_contact_run(cc::PhysicsContact& contact);
    virtual void update();
};

END_COMPONENTS_NS
END_ENTITIES_NS

#endif