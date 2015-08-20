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
bool flip_weapon = false;

//public
void BulletAimerComponent::init() {
    type = UNIT_COMPONENT_TYPE_BULLET_AIMER;

    weapon = Sprite::create();
    weapon->setAnchorPoint(Vec2(.5f, .5f));
    weapon->setVisible(false);
    root::map_layer->addChild(weapon, 4);

    equip_weapon(0);
}

BulletAimerComponent::~BulletAimerComponent() {
    cleanup();
}

void BulletAimerComponent::cleanup() {
    if (is_removal_scheduled()) return;

    root::map_layer->removeChild(weapon);
    schedule_removal();
}

void BulletAimerComponent::equip_weapon(int _weapon_id) { //temp argument
    weapon_id = _weapon_id;
    if (weapon_id == 0) {
        weapon->setTexture(assets::textures::laser_machine_gun);
        weapon->setScale(.2f);
        rotation_offset = -180;
        flip_weapon = true;
    }else if (weapon_id == 1) {
        weapon->setTexture(assets::textures::c4);
        weapon->setScale(.55f);
        rotation_offset = 90;
        flip_weapon = false;
    }
    weapon->setTextureRect(Rect(0, 0, weapon->getTexture()->getContentSize().width, weapon->getTexture()->getContentSize().height));
}

void BulletAimerComponent::update() {
    gui::game::set_power_text(power);

    weapon->setPosition(Vec2(current_unit->base->getPositionX(), current_unit->base->getPositionY()));

    if (aiming) {
        float x = (root::scene->screen_size.width) / 2.0f;
        float y = (root::scene->screen_size.height) / 2.0f;
        float angle = atan2(-input::get_mouse_pos().y - y, input::get_mouse_pos().x - x) * (180 / M_PI);
        weapon->setRotation(angle + rotation_offset);
        if (flip_weapon) {
            weapon->setFlippedY(angle >= -90 && angle <= 90);
        }

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
            float r = -angle;

            if (weapon_id == 0) {
                b->add_logic_fire_bullet(r, power);
            }else if (weapon_id == 1) {
                b->add_logic_c4(r, power);
            }

            gui::game::wait_for_bullet(g);
            map::camera::follow_bullet(g);

            cleanup();
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