#include "entities/units/components/PlayerMoveComponent.h"

#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsContact.h>
#include <physics/CCPhysicsWorld.h>

#include "entities/units/components/BulletAimerComponent.h"
#include "entities/units/components/ColliderComponent.h"
#include "entities/units/Unit.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "states/Game.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS
BEGIN_COMPONENTS_NS

//private

//public
void PlayerMoveComponent::init() {
    bullet_aimer = ref->get_component<BulletAimerComponent>();
    collider = ref->get_component<ColliderComponent>();
}

bool PlayerMoveComponent::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (states::game::terrain->is_terrain(a, b)) {

    }

    return false;
}

void PlayerMoveComponent::update() {
    if (moving) {
        ref->base->setRotation(cos(root::time_since_startup * 15.0f) * 15.0f);
    }else {
        ref->base->setRotation(0.0f);
    }

    moving = false;
    if (!jumping && (bullet_aimer == NULL || !bullet_aimer->aiming)) {
        if (input::key_down(EventKeyboard::KeyCode::KEY_D)) {
            ref->pbody->setVelocity(Vec2(move_vel_x, ref->pbody->getVelocity().y));
            //pbody->applyImpulse(Vec2(400.0f, 0));
            facing_right = true;
            ref->base->setFlippedX(facing_right);
            moving = true;
        }
        if (input::key_down(EventKeyboard::KeyCode::KEY_A)) {
            ref->pbody->setVelocity(Vec2(-move_vel_x, ref->pbody->getVelocity().y));
            //pbody->applyImpulse(Vec2(-400.0f, 0));
            facing_right = false;
            ref->base->setFlippedX(facing_right);
            moving = true;
        }
        if (collider->colliding_with_delay && input::key_pressed(EventKeyboard::KeyCode::KEY_W)) {
            ref->pbody->setVelocity(Vec2(ref->pbody->getVelocity().x, 600.0f));
            if (facing_right) ref->pbody->setVelocity(Vec2(jump_vel_x, ref->pbody->getVelocity().y));
            else ref->pbody->setVelocity(Vec2(-jump_vel_x, ref->pbody->getVelocity().y));

            jumping = true;
        }
    }

    if (jumping && ref->pbody->getVelocity().x <= 2 && ref->pbody->getVelocity().x >= -2) {
        jumping = false;
    }

    //pbody->setVelocity(Vec2(clampf(pbody->getVelocity().x, -200.0f, 200.0f), pbody->getVelocity().y));
}

END_COMPONENTS_NS
END_UNITS_NS
END_ENTITIES_NS