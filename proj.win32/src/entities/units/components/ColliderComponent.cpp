#include "entities/units/components/ColliderComponent.h"

#include <physics/CCPhysicsContact.h>
#include <physics/CCPhysicsWorld.h>

#include "states/Game.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_COMPONENTS_NS

//private

//public
void ColliderComponent::init() {

}

bool ColliderComponent::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (states::game::terrain->is_terrain(a, b)) {
        colliding_with_delay = true;
        colliding = true;
        collide_timer = 0;
        return true;
    }

    return false;
}

void ColliderComponent::update() {
    if (++collide_timer >= 10) {
        collide_timer = 0;
        colliding = false;
        colliding_with_delay = false;
    }
}

END_COMPONENTS_NS
END_ENTITIES_NS