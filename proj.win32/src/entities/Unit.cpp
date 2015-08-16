#include "entities/Unit.h"

#include <2d/CCCamera.h>
#include <physics/CCPhysicsBody.h>
#include <base/CCEventDispatcher.h>
#include <physics/CCPhysicsWorld.h>
#include <chipmunk.h>

#include "assets/Textures.h"
#include "debug/Logger.h"
#include "entities/Bullet.h"
#include "input/KeyboardInput.h"
#include "input/MouseInput.h"
#include "physics/Physics.h"
#include "states/Game.h"
#include "StateManager.h"

BEGIN_ENTITIES_NS

using namespace cocos2d;

std::vector<Unit*> units;
float move_vel_x = 200.0f;
float jump_vel_x = 450.0f;
const float LARGE_VEL_X = 100000.0f;
bool jumping = false;
bool colliding = false;
int collide_timer = 0;
bool moving = false;
bool aiming = false;

Sprite* cone;
const float MIN_POWER = .5f;
const float MAX_POWER = 1;
float power = MIN_POWER;

Unit::Unit() {
    base = Sprite::createWithTexture(assets::textures::duck);
    base->setPosition(-400, 0);
    base->retain();
    root::scene->addChild(base, 1);
    
    cone = Sprite::create("cone.png");
    cone->setAnchorPoint(Vec2(.5f, -.25f));
    cone->setVisible(false);
    root::scene->addChild(cone, 1);

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

    physics::add_on_contact_run(CC_CALLBACK_1(Unit::on_contact_run, this), this);

    dest_x = base->getPositionX();
    dest_y = base->getPositionY();
    dest_rota = 0;

    units.push_back(this);
}

bool Unit::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        if (a == base || b == base) {
            if (b == states::game::terrain->base || a == states::game::terrain->base) {
                can_jump = true;
                colliding = true;
                collide_timer = 0;
                return true;
            }
        }
    }

    return false;
}

void Unit::update() {
    if (!player_input) return;

    cone->setPosition(Vec2(base->getPositionX(), base->getPositionY()));

    if (aiming) {
        float x = (root::scene->screen_size.width) / 2.0f;
        float y = (base->getContentSize().height + root::scene->screen_size.height) / 2.0f;
        cone->setRotation(atan2(-input::get_mouse_pos().y - y, input::get_mouse_pos().x - x) * (180 / M_PI) + 90);

        if (input::key_down(EventKeyboard::KeyCode::KEY_W)) {
            power += .05f;
            power = clampf(power, MIN_POWER, MAX_POWER);
        }else if (input::key_down(EventKeyboard::KeyCode::KEY_S)) {
            power -= .05f;
            power = clampf(power, MIN_POWER, MAX_POWER);
        }

        if (input::get_mouse_button_pressed(MOUSE_BUTTON_LEFT)) {
            auto b = bullet::create_bullet(base->getPositionX(), base->getPositionY());
            b->add_logic_test(-cone->getRotation() + 90, power);
        }
    }

    if (input::get_mouse_button_pressed(MOUSE_BUTTON_RIGHT)) {
        aiming = !aiming;
        cone->setVisible(aiming);
    }

    if (moving) {
        base->setRotation(cos(root::time_since_startup * 15.0f) * 15.0f);
    }else {
        base->setRotation(0.0f);
    }

    moving = false;
    if (!jumping && !aiming) {
        if (input::key_down(EventKeyboard::KeyCode::KEY_D)) {
            pbody->setVelocity(Vec2(move_vel_x, pbody->getVelocity().y));
            //pbody->applyImpulse(Vec2(400.0f, 0));
            facing_right = true;
            base->setFlippedX(facing_right);
            moving = true;
        }
        if (input::key_down(EventKeyboard::KeyCode::KEY_A)) {
            pbody->setVelocity(Vec2(-move_vel_x, pbody->getVelocity().y));
            //pbody->applyImpulse(Vec2(-400.0f, 0));
            facing_right = false;
            base->setFlippedX(facing_right);
            moving = true;
        }
        if (can_jump && input::key_pressed(EventKeyboard::KeyCode::KEY_W)) {
            pbody->setVelocity(Vec2(pbody->getVelocity().x, 600.0f));
            if (facing_right) pbody->setVelocity(Vec2(jump_vel_x, pbody->getVelocity().y));
            else pbody->setVelocity(Vec2(-jump_vel_x, pbody->getVelocity().y));

            can_jump = false;
            jumping = true;
        }
    }

    if (jumping && pbody->getVelocity().x <= 2 && pbody->getVelocity().x >= -2) {
        jumping = false;
    }

    if (++collide_timer >= 10) {
        collide_timer = 0;
        colliding = false;
        can_jump = false;
    }

    //pbody->setVelocity(Vec2(clampf(pbody->getVelocity().x, -200.0f, 200.0f), pbody->getVelocity().y));
}

void test_peer_join(network::peers::Peer* peer) {
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
