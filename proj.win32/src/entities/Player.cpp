#include "Player.h"
#include "../StateManager.h"
#include "input/SimpleInput.h"

using entity::Player;

Player* entity::player;

Player::Player() {
    base = cc::Sprite::create("HelloWorld.png");
    base->setPosition(base->getContentSize().width, base->getContentSize().height);
    base->setScale(.25f);
    base->retain();
    state::scene->addChild(base, 1);
    
    pbody = cc::PhysicsBody::createBox(cc::Size(base->getContentSize().width * base->getScaleX(),
        base->getContentSize().height * base->getScaleY()));
    base->setPhysicsBody(pbody);

    //scene->p_world->setDebugDrawMask(cc::PhysicsWorld::DEBUGDRAW_ALL);

    pbody->setCategoryBitmask(0x04);
    pbody->setCollisionBitmask(0x02);
    pbody->setContactTestBitmask(0x02);
}

void Player::update() {
    pbody->applyImpulse(cc::Vec2(0, -10000.0f));
    pbody->setVelocityLimit(500.0f);
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_RIGHT_ARROW)) {
        pbody->applyImpulse(cc::Vec2(10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_LEFT_ARROW)) {
        pbody->applyImpulse(cc::Vec2(-10000.0f, 0));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_UP_ARROW)) {
        pbody->applyImpulse(cc::Vec2(0, 40000.0f));
    }
    if (input::key_down(cc::EventKeyboard::KeyCode::KEY_DOWN_ARROW)) {
        pbody->applyImpulse(cc::Vec2(0, -40000.0f));
    }
}