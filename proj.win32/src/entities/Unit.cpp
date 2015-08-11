#include "entities/Unit.h"

#include <2d/CCCamera.h>
#include <physics/CCPhysicsBody.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>
#include <chipmunk.h>

#include "debug/Logger.h"
#include "input/SimpleInput.h"
#include "StateManager.h"

namespace entities {

    using namespace cocos2d;

    Unit* test_player;

    std::vector<Unit*> units;
    float max_vel_x;
    float move_vel_x = 200.0f;
    float jump_vel_x = 400.0f;
    const float LARGE_VEL_X = 100000.0f;
    bool jumping = false;

    Unit::Unit() {
        base = Sprite::create("duck.png");
        base->setPosition(0, 0);
        base->retain();
        root::scene->addChild(base, 1);

        PhysicsMaterial mat;
        mat.density = .1f;
        mat.friction = 4;
        mat.restitution = 0;
        pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
            base->getContentSize().height * base->getScaleY()), mat);
        pbody->setCollisionBitmask(1);
        pbody->setContactTestBitmask(true);
        pbody->setRotationEnable(false);
        pbody->setPositionOffset(Vec2(0, -10));
        root::scene->p_world->setGravity(Vec2(0, -1200.0f));
        base->setPhysicsBody(pbody);

        max_vel_x = move_vel_x;

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
        if (player_input) {
            if (!jumping) {
                if (input::key_down(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
                    pbody->applyImpulse(Vec2(LARGE_VEL_X, 0));
                    facing_right = true;
                    base->setFlippedX(true);
                }
                if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
                    pbody->applyImpulse(Vec2(-LARGE_VEL_X, 0));
                    facing_right = false;
                    base->setFlippedX(false);
                }
                if (can_jump && input::key_pressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
                    pbody->setVelocity(Vec2(pbody->getVelocity().x, 700.0f));
                    if (facing_right) pbody->applyImpulse(Vec2(LARGE_VEL_X, 0));
                    else pbody->applyImpulse(Vec2(-LARGE_VEL_X, 0));
                    max_vel_x = jump_vel_x;

                    can_jump = false;
                    jumping = true;
                }
            }
        }else {
            pbody->setGravityEnable(false);
            base->setPosition(base->getPositionX() + ((dest_x - base->getPositionX()) / 2.0f), (base->getPositionY() + (dest_y - base->getPositionY()) / 2.0f));
            base->setRotation(base->getRotation() + ((dest_rota - base->getRotation()) / 2.0f));
        }

        if (pbody->getVelocity().x > max_vel_x) pbody->setVelocity(Vec2(max_vel_x, pbody->getVelocity().y));
        else if (pbody->getVelocity().x < -max_vel_x) pbody->setVelocity(Vec2(-max_vel_x, pbody->getVelocity().y));

        if (pbody->getVelocity().x <= 2 && pbody->getVelocity().x > -2) {
            jumping = false;
            max_vel_x = move_vel_x;
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
