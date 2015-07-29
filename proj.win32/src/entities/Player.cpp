#include "Player.h"
#include "../StateManager.h"

using entity::Player;

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

}