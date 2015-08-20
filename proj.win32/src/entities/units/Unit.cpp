#include "entities/units/Unit.h"

#include <physics/CCPhysicsContact.h>

#include "assets/Textures.h"
#include "debug/Logger.h"
#include "entities/bullets/Bullet.h"
#include "entities/units/UnitSpawner.h"
#include "entities/units/components/PlayerMoveComponent.h"
#include "entities/units/components/BulletAimerComponent.h"
#include "entities/units/components/ColliderComponent.h"
#include "gui/GameGUI.h"
#include "physics/Physics.h"
#include "states/Game.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

using namespace cocos2d;

//public
Unit::Unit(UnitTeam* _team, UnitType _type) {
    team = _team;
    type = _type;

    if (type == UNIT_TYPE_CAPTAIN) {
        max_health = 25.0f;
    }else {
        max_health = 12.0f;
    }
    health = max_health;

    base = Sprite::createWithTexture(assets::textures::duck);
    base->setScale(type == UNIT_TYPE_CAPTAIN ? .65f : .5f);
    root::map_layer->addChild(base, 2);

    shadow = Sprite::createWithTexture(assets::textures::unit_shadow);
    shadow->setScale(base->getScale());
    shadow->setColor(team->get_colour());
    root::map_layer->addChild(shadow, 1);

    PhysicsMaterial mat;
    mat.density = 0.0f;
    mat.friction = 40.0f;
    mat.restitution = .5f;
    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
                                        base->getContentSize().height * base->getScaleY()), mat);
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setRotationEnable(false);
    //pbody->setPositionOffset(Vec2(0, -10));
    //pbody->setGravityEnable(false);
    pbody->setVelocityLimit(1000.0f);
    pbody->setMass(100.0f);
    base->setPhysicsBody(pbody);

    physics::add_on_contact_run(CC_CALLBACK_1(Unit::on_contact_run, this), this);

    add_component<components::ColliderComponent>()->init();
}

bool Unit::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        if (a == base || b == base) {
            //loop through all component on_contact_run functions (if one exists)
            //if a collision occurred, true is returned
            bool collided = false;
            for (auto& c : components) {
                if (c->on_contact_run(contact)) collided = true;
            }
            if (collided) return true;
        }
    }

    return false;
}

Unit::~Unit() {
    pbody->release();
    root::map_layer->removeChild(base);
    root::map_layer->removeChild(shadow);
    physics::remove_on_contact_run(this);

    components.clear();
}

void Unit::equip_weapon(int weapon_id) { //temp argument
    auto& c = get_component<components::BulletAimerComponent>();
    if (c) {
        c->equip_weapon(weapon_id);
    }
}

void Unit::update() {
    update_scheduler();

    shadow->setPosition(base->getPosition());

    for (int n = 0; n < components.size(); ++n) {
        if (!components[n]->is_removal_scheduled()) components[n]->update();
        if (components[n]->is_removal_scheduled()) {
            components[n]->cleanup();
            components.erase(components.begin() + n, components.begin() + n + 1);
            --n;
        }
    }
}

void Unit::take_damage(float damage) {
    health -= damage;
    if (health <= 0) {
        schedule_removal();
    }
    gui::game::sort_ui_bars();
}

//-- begin template definitions --

template <typename T> std::shared_ptr<T> Unit::add_component() {
    for (auto& c : components) {
        if (typeid(*c.get()) == typeid(T)) {
            log_error << "attempted to add a component when another one of the same type already existed. \
                          cannot have more than 1 component";
        }
    }

    std::shared_ptr<T> t(new T(this));
    components.push_back(t);
    return t;
}
template components::PlayerMoveComponentPtr       Unit::add_component<components::PlayerMoveComponent>();
template components::BulletAimerComponentPtr      Unit::add_component<components::BulletAimerComponent>();
template components::ColliderComponentPtr         Unit::add_component<components::ColliderComponent>();

template <typename T> std::shared_ptr<T> Unit::get_component() {
    for (auto& c : components) {
        if (typeid(*c.get()) == typeid(T)) {
            return std::static_pointer_cast<T>(c);
        }
    }
    return NULL;
}
template components::PlayerMoveComponentPtr       Unit::get_component<components::PlayerMoveComponent>();
template components::BulletAimerComponentPtr      Unit::get_component<components::BulletAimerComponent>();
template components::ColliderComponentPtr         Unit::get_component<components::ColliderComponent>();

template <typename T> void Unit::remove_component() {
    for (int n = 0; n < components.size(); ++n) {
        if (typeid(*components[n].get()) == typeid(T)) {
            components.erase(components.begin() + n, components.begin() + n + 1);
            return;
        }
    }
}
template void Unit::remove_component<components::PlayerMoveComponent>();
template void Unit::remove_component<components::BulletAimerComponent>();
template void Unit::remove_component<components::ColliderComponent>();

//-- end component template definitions --

END_UNITS_NS
END_ENTITIES_NS
