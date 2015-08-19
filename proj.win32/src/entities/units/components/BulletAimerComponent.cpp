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
float rotation_offset = 0;

//public
void BulletAimerComponent::init() {
    type = UNIT_COMPONENT_TYPE_BULLET_AIMER;

    weapon = Sprite::createWithTexture(assets::textures::laser_machine_gun);
    weapon->setAnchorPoint(Vec2(.5f, .5f));
    weapon->setVisible(false);
    weapon->setScale(.2f);
    rotation_offset = 180;
    root::map_layer->addChild(weapon, 4);
}

BulletAimerComponent::~BulletAimerComponent() {
    root::map_layer->removeChild(weapon);
}

void BulletAimerComponent::update() {
    gui::game::set_power_text(power);

    weapon->setPosition(Vec2(current_unit->base->getPositionX(), current_unit->base->getPositionY()));

    if (aiming) {
        float x = (root::scene->screen_size.width) / 2.0f;
        float y = (root::scene->screen_size.height) / 2.0f;
        weapon->setRotation(atan2(-input::get_mouse_pos().y - y, input::get_mouse_pos().x - x) * (180 / M_PI) + rotation_offset);
        weapon->setFlippedY(weapon->getRotation() >= 90 && weapon->getRotation() <= 270);

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
            b->add_logic_fire_bullet(-weapon->getRotation() + 180, power);
            gui::game::wait_for_bullet(g);
            map::camera::follow_bullet(g);
            schedule_removal();
        }
    }

    if (input::get_mouse_button_pressed(MOUSE_BUTTON_RIGHT)) {
        aiming = !aiming;
        weapon->setVisible(aiming);
    }
}
END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS