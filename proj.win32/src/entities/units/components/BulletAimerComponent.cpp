#include "entities/units/components/BulletAimerComponent.h"

#include "assets/Textures.h"
#include "debug/Logger.h"
#include "entities/units/components/ComponentBase.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "entities/bullets/Bullet.h"
#include "entities/bullets/BulletGroup.h"
#include "gui/GameGUI.h"
#include "input/MouseInput.h"
#include "input/KeyboardInput.h"
#include "map/Cam.h"
#include "states/Game.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

//private

//public
void BulletAimerComponent::init() {
    type = UNIT_COMPONENT_TYPE_BULLET_AIMER;

    cone = Sprite::createWithTexture(assets::textures::cone);
    cone->setAnchorPoint(Vec2(.5f, -.25f));
    cone->setVisible(false);
    root::map_layer->addChild(cone, 1);
}

BulletAimerComponent::~BulletAimerComponent() {
    root::map_layer->removeChild(cone);
}

void BulletAimerComponent::update() {
    gui::game::set_power_text(power);

    cone->setPosition(Vec2(current_unit->base->getPositionX(), current_unit->base->getPositionY()));

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
            auto g = bullets::create_group(ref);
            auto b = g->create_bullet(ref->base->getPositionX(), ref->base->getPositionY());
            b->add_logic_test(-cone->getRotation() + 90, power);
            gui::game::wait_for_bullet(g);
            map::camera::follow_bullet(g);
            schedule_removal();
        }
    }

    if (input::get_mouse_button_pressed(MOUSE_BUTTON_RIGHT)) {
        aiming = !aiming;
        cone->setVisible(aiming);
    }
}
END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS