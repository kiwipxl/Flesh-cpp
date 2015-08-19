#ifndef _COMPONENTS_BULLET_AIMER_H_
#define _COMPONENTS_BULLET_AIMER_H_

#include <2d/CCSprite.h>

#include "entities/units/components/ComponentBase.h"

namespace cocos2d {

};
namespace cc = cocos2d;

BEGIN_ENTITIES_NS

class Unit;

BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

class BulletAimerComponent : public ComponentBase {

public:
    BulletAimerComponent(Unit* _ref) { ref = _ref; }
    virtual ~BulletAimerComponent();

    cc::Sprite* cone;
    bool aiming = false;

    const float MIN_POWER = .5f;
    const float MAX_POWER = 1.25f;
    float power = MIN_POWER;

    void init();
    virtual void update();
};

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS

#endif