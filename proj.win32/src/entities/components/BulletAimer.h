#ifndef _COMPONENTS_BULLET_AIMER_H_
#define _COMPONENTS_BULLET_AIMER_H_

#include <2d/CCSprite.h>

#include "assets/Textures.h"
#include "entities/components/ComponentBase.h"
#include "entities/Unit.h"
#include "input/MouseInput.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS
BEGIN_COMPONENTS_NS

using namespace cocos2d;

class BulletAimerComponent : public ComponentBase {

public:
    Sprite* cone;
    bool aiming = false;

    const float MIN_POWER = .5f;
    const float MAX_POWER = 1;
    float power = MIN_POWER;

    BulletAimerComponent(Unit* _ref) {
        ref = _ref;
        cone = Sprite::createWithTexture(assets::textures::cone);
        cone->setAnchorPoint(Vec2(.5f, -.25f));
        root::map_layer->addChild(cone, 1);
    }

    virtual void update() {
        cone->setPosition(Vec2(entities::current_unit->base->getPositionX(), entities::current_unit->base->getPositionY()));

        if (aiming) {
            float x = (root::scene->screen_size.width) / 2.0f;
            float y = (ref->base->getContentSize().height + root::scene->screen_size.height) / 2.0f;
            cone->setRotation(atan2(-input::get_mouse_pos().y - y, input::get_mouse_pos().x - x) * (180 / M_PI) + 90);

            if (input::key_down(EventKeyboard::KeyCode::KEY_W)) {
                power += .05f;
                power = clampf(power, MIN_POWER, MAX_POWER);
            }else if (input::key_down(EventKeyboard::KeyCode::KEY_S)) {
                power -= .05f;
                power = clampf(power, MIN_POWER, MAX_POWER);
            }

            if (input::get_mouse_button_pressed(MOUSE_BUTTON_LEFT)) {
                //auto b = bullet::create_bullet(base->getPositionX(), base->getPositionY(), this);
                //b->add_logic_test(-cone->getRotation() + 90, power);
            }
        }

        if (input::get_mouse_button_pressed(MOUSE_BUTTON_RIGHT)) {
            aiming = !aiming;
            cone->setVisible(aiming);
        }
    }
};

END_COMPONENTS_NS
END_ENTITIES_NS

#endif