#include "entities/Unit.h"

#include <2d/CCCamera.h>
#include <physics/CCPhysicsBody.h>
#include <physics/CCPhysicsWorld.h>
#include <chipmunk.h>

#include "assets/Textures.h"
#include "debug/Logger.h"
#include "entities/Bullet.h"
#include "entities/components/PlayerMove.h"
#include "entities/components/BulletAimer.h"
#include "input/KeyboardInput.h"
#include "physics/Physics.h"
#include "states/Game.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS

using namespace cocos2d;

//public
std::vector<Unit*> units;

//private
Unit* current_unit;
int current_unit_index;

Unit::Unit() {
    base = Sprite::createWithTexture(assets::textures::duck);
    base->retain();
    root::map_layer->addChild(base, 1);

    PhysicsMaterial mat;
    mat.density = 0.0f;
    mat.friction = 40.0f;
    mat.restitution = .5f;
    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
                                        base->getContentSize().height * base->getScaleY()), mat);
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setRotationEnable(false);
    pbody->setPositionOffset(Vec2(0, -10));
    //pbody->setGravityEnable(false);
    root::scene->p_world->setAutoStep(false);
    root::scene->p_world->setGravity(Vec2(0, -980.0f));
    base->setPhysicsBody(pbody);

    //physics::add_on_contact_run(CC_CALLBACK_1(Unit::on_contact_run, this), this);

    units.push_back(this);
}

template <typename T> T* Unit::add_component() {
    T* t = new T(this);
    components.push_back(t);
    return t;
}
template components::PlayerMoveComponent* Unit::add_component<components::PlayerMoveComponent>();
template components::BulletAimerComponent* Unit::add_component<components::BulletAimerComponent>();

template <typename T> T* Unit::get_component() {
    for (auto* c : components) {
        if (typeid(*c) == typeid(T)) {
            return (T*)c;
        }
    }
    return NULL;
}
template components::PlayerMoveComponent* Unit::get_component<components::PlayerMoveComponent>();
template components::BulletAimerComponent* Unit::get_component<components::BulletAimerComponent>();

template <typename T> void Unit::remove_component() {
    for (auto* c : components) {

    }
}
template void Unit::remove_component<components::PlayerMoveComponent>();
template void Unit::remove_component<components::BulletAimerComponent>();

Unit::~Unit() {
    pbody->release();
    root::map_layer->removeChild(base);
}

void Unit::update() {
    for (int n = 0; n < components.size(); ++n) {
        components[n]->update();
    }
}

void next_unit() {
    current_unit->player_input = false;
    ++current_unit_index;
    if (current_unit_index >= units.size()) {
        current_unit_index = 0;
    }
    current_unit = units[current_unit_index];
    current_unit->player_input = true;
}

void select_current_unit() {
    current_unit = units[current_unit_index];
    current_unit->player_input = true;
}

void update_units() {
    for (int n = 0; n < units.size(); ++n) {
        if (!units[n]->is_scheduled_removal()) units[n]->update();
        if (units[n]->is_scheduled_removal()) {
            delete units[n];
            units.erase(units.begin() + n, units.begin() + n + 1);
            --n;
            select_current_unit();
        }
    }
}

END_ENTITIES_NS
