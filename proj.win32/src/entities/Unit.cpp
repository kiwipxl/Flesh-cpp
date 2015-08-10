#include "entities/Unit.h"

#include <2d/CCCamera.h>
#include <physics/CCPhysicsBody.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>

#include "debug/Logger.h"
#include "input/SimpleInput.h"
#include "StateManager.h"

namespace entities {

    using namespace cocos2d;

    Unit* test_player;

    std::vector<Unit*> units;
    float max_vel_x = 400.0f;

    Unit::Unit() {
        base = Sprite::create("duck.png");
        base->setPosition(0, 0);
        base->retain();
        root::scene->addChild(base, 1);

        pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
            base->getContentSize().height * base->getScaleY()));
        pbody->setCollisionBitmask(1);
        pbody->setContactTestBitmask(true);
        pbody->setRotationEnable(false);
        pbody->setPositionOffset(Vec2(0, -10));
        pbody->setAngularDamping(10000.0f);
        root::scene->p_world->setGravity(Vec2(0, -800.0f));
        base->setPhysicsBody(pbody);

        root::scene->p_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

        auto contact_listener = EventListenerPhysicsContact::create();
        contact_listener->onContactPreSolve = CC_CALLBACK_1(Unit::physics_contact, this);
        root::scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contact_listener, base);
        root::scene->p_world->setUpdateRate(0.0f);

        dest_x = base->getPositionX();
        dest_y = base->getPositionY();
        dest_rota = 0;
    }

    bool Unit::physics_contact(PhysicsContact& contact) {
        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();
        if (a && b) {
            can_jump = true;
        }

        return true;
    }

    void Unit::update() {
        pbody->setLinearDamping(1.0f);
        if (pbody->getVelocity().x > max_vel_x) pbody->setVelocity(Vec2(max_vel_x, pbody->getVelocity().y));
        else if (pbody->getVelocity().x < -max_vel_x) pbody->setVelocity(Vec2(-max_vel_x, pbody->getVelocity().y));

        if (player_input) {
            if (input::key_down(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
                pbody->applyImpulse(Vec2(10000.0f, 0));
                facing_right = true;
                base->setFlippedX(true);
            }
            if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
                pbody->applyImpulse(Vec2(-10000.0f, 0));
                facing_right = false;
                base->setFlippedX(false);
            }
            if (can_jump && input::key_pressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
                pbody->setVelocity(Vec2(pbody->getVelocity().x, 700.0f));
                if (facing_right) pbody->applyImpulse(Vec2(250000.0f, 0));
                else pbody->applyImpulse(Vec2(-250000.0f, 0));

                can_jump = false;
            }
        }else {
            pbody->setGravityEnable(false);
            base->setPosition(base->getPositionX() + ((dest_x - base->getPositionX()) / 2.0f), (base->getPositionY() + (dest_y - base->getPositionY()) / 2.0f));
            base->setRotation(base->getRotation() + ((dest_rota - base->getRotation()) / 2.0f));
        }

        can_jump = false;
    }

    void test_peer_join(network::peers::Peer* peer) {
        test_player->base->setPosition(0, 0);
        Unit* unit = new Unit();
        unit->peer = peer;
        units.push_back(unit);
    }

    void test_peer_movement(network::peers::Peer* peer, int x, int y, float rota) {
        for (int n = 0; n < units.size(); ++n) {
            if (units[n]->peer == peer) {
                units[n]->dest_x = x;
                units[n]->dest_y = y;
                units[n]->dest_rota = rota;
            }
        }
    }

    void update_units() {
        for (int n = 0; n < units.size(); ++n) {
            units[n]->update();
        }
    }
};
