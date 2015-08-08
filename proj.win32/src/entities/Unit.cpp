#include "entities/Unit.h"

#include <2d/CCCamera.h>
#include <physics/CCPhysicsBody.h>

#include "StateManager.h"
#include "input/SimpleInput.h"

namespace entities {

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
        base->setPhysicsBody(pbody);

        //state::scene->p_world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

        //pbody->setCategoryBitmask(0x04);
        //pbody->setCollisionBitmask(0x02);
        //pbody->setContactTestBitmask(0x02);

        dest_x = base->getPositionX();
        dest_y = base->getPositionY();
        dest_rota = 0;
    }

    void Unit::update() {
        pbody->applyImpulse(Vec2(0, -10000.0f));
        pbody->setVelocityLimit(5000.0f);
        if (player_input) {
            if (input::key_down(EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
                pbody->applyImpulse(Vec2(10000.0f, 0));
            }
            if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
                pbody->applyImpulse(Vec2(-10000.0f, 0));
            }
            if (input::key_down(EventKeyboard::KeyCode::KEY_UP_ARROW)) {
                pbody->applyImpulse(Vec2(0, 40000.0f));
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

    void test_peer_join(peers::Peer* peer) {
        test_player->base->setPosition(0, 0);
        Unit* unit = new Unit();
        unit->peer = peer;
        units.push_back(unit);
    }

    void test_peer_movement(peers::Peer* peer, int x, int y, float rota) {
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
