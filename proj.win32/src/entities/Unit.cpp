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

    Unit::Unit() {
        base = Sprite::create("HelloWorld.png");
        base->setPosition(base->getContentSize().width, base->getContentSize().height);
        base->setCameraMask((u_short)CameraFlag::USER1);
        base->setScale(.25f);
        base->retain();
        root::scene->addChild(base, 1);

        pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
            base->getContentSize().height * base->getScaleY()));
        pbody->setCollisionBitmask(1);
        pbody->setContactTestBitmask(true);
        pbody->setRotationEnable(false);
        pbody->setPositionOffset(Vec2(0, -10));
        root::scene->p_world->setGravity(Vec2(0, -1000));
        base->setPhysicsBody(pbody);

        //state::scene->p_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

        auto contact_listener = EventListenerPhysicsContact::create();
        contact_listener->onContactBegin = CC_CALLBACK_1(Unit::physics_contact, this);
        root::scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contact_listener, base);

        dest_x = base->getPositionX();
        dest_y = base->getPositionY();
        dest_rota = 0;
    }

    bool Unit::physics_contact(PhysicsContact& contact) {
        auto a = contact.getShapeA()->getBody()->getNode();
        auto b = contact.getShapeB()->getBody()->getNode();
        if (a && b) {
            can_jump = true;
            if (a == base) {
                int s = 0;
            }
        }

        return true;
    }

    void Unit::update() {
        log_info << pbody->getLinearDamping();
        //pbody->applyImpulse(Vec2(0, -20000.0f));
        //pbody->setVelocityLimit(5000.0f);
        if (player_input) {
            if (input::key_down(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
                pbody->applyImpulse(Vec2(10000.0f, 0));
                facing_right = true;
            }
            if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
                pbody->applyImpulse(Vec2(-10000.0f, 0));
                facing_right = false;
            }
            if (can_jump && input::key_pressed(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
                pbody->setVelocity(Vec2(pbody->getVelocity().x, 500.0f));
                if (facing_right) pbody->applyImpulse(Vec2(200000.0f, 0));
                else pbody->applyImpulse(Vec2(-200000.0f, 0));

                can_jump = false;
            }
            if (input::key_down(EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
                pbody->applyImpulse(Vec2(0, -40000.0f));
            }
        }else {
            pbody->setGravityEnable(false);
            base->setPosition(base->getPositionX() + ((dest_x - base->getPositionX()) / 2.0f), (base->getPositionY() + (dest_y - base->getPositionY()) / 2.0f));
            base->setRotation(base->getRotation() + ((dest_rota - base->getRotation()) / 2.0f));
        }
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
