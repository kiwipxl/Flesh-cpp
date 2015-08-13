#include "entities/Unit.h"

#include <2d/CCCamera.h>
#include <physics/CCPhysicsBody.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>

#include "assets/Textures.h"
#include "debug/Logger.h"
#include "entities/Bullet.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS

using namespace cocos2d;

Unit* test_player;

std::vector<Unit*> units;
float move_vel_x = 200.0f;
float jump_vel_x = 400.0f;
const float LARGE_VEL_X = 100000.0f;
bool jumping = false;
bool colliding = false;
int collide_timer = 0;
bool moving = false;

Unit::Unit() {
    base = Sprite::create();
    base->setTexture(assets::textures::duck);
    base->setPosition(0, 0);
    base->retain();
    root::scene->addChild(base, 1);

    PhysicsMaterial mat;
    mat.density = .1f;
    mat.friction = 4;
    mat.restitution = 0.0f;
    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
        base->getContentSize().height * base->getScaleY()), mat);
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setRotationEnable(false);
    pbody->setPositionOffset(Vec2(0, -10));
    //pbody->setGravityEnable(false);
    root::scene->p_world->setGravity(Vec2(0, -1200.0f));
    base->setPhysicsBody(pbody);

    auto contact_listener = EventListenerPhysicsContact::create();
    contact_listener->onContactPreSolve = CC_CALLBACK_1(Unit::physics_contact, this);
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
        colliding = true;
        collide_timer = 0;
    }

    return true;
}

void Unit::update() {
    if (input::get_mouse_button_down(MOUSE_BUTTON_LEFT)) {
        facing_right = !facing_right;
        base->setFlippedX(facing_right);
        create_bullet();
    }

    if (colliding && moving) {
        base->setRotation(cos(root::time_since_startup * 15.0f) * 15.0f);
    }else {
        base->setRotation(0.0f);
    }

    if (player_input) {
        moving = false;
        if (!jumping) {
            if (input::key_down(EventKeyboard::KeyCode::KEY_D)) {
                pbody->setVelocity(Vec2(move_vel_x, pbody->getVelocity().y));
                facing_right = true;
                base->setFlippedX(facing_right);
                moving = true;
            }
            if (input::key_down(EventKeyboard::KeyCode::KEY_A)) {
                pbody->setVelocity(Vec2(-move_vel_x, pbody->getVelocity().y));
                facing_right = false;
                base->setFlippedX(facing_right);
                moving = true;
            }
            if (can_jump && input::key_pressed(EventKeyboard::KeyCode::KEY_W)) {
                pbody->setVelocity(Vec2(pbody->getVelocity().x, 700.0f));
                if (facing_right) pbody->setVelocity(Vec2(jump_vel_x, pbody->getVelocity().y));
                else pbody->setVelocity(Vec2(-jump_vel_x, pbody->getVelocity().y));

                can_jump = false;
                jumping = true;
            }
        }
    }else {
        pbody->setGravityEnable(false);
        base->setPosition(base->getPositionX() + ((dest_x - base->getPositionX()) / 2.0f), (base->getPositionY() + (dest_y - base->getPositionY()) / 2.0f));
        base->setRotation(base->getRotation() + ((dest_rota - base->getRotation()) / 2.0f));
    }

    if (pbody->getVelocity().x <= 2 && pbody->getVelocity().x > -2) {
        jumping = false;
    }

    if (++collide_timer >= 6) {
        collide_timer = 0;
        colliding = false;
        can_jump = false;
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

END_ENTITIES_NS
