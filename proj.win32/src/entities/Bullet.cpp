#include "entities/Bullet.h"

#include <physics/CCPhysicsBody.h>
#include <base/CCEventDispatcher.h>

#include "assets/Textures.h"
#include "SceneManager.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

//public

//private
std::vector<BulletPtr> bullets;

BulletPtr create_bullet(BulletType type, int x, int y) {
    BulletPtr b(new Bullet(type, x, y));
    bullets.push_back(b);
    return b;
}

void update() {
    for (int n = 0; n < bullets.size(); ++n) {
        bullets[n]->update();
        if (bullets[n]->is_removal_scheduled()) {
            bullets.erase(bullets.begin() + n, bullets.begin() + n + 1);
            --n;
        }
    }
}

//-- begin Bullet class --

Bullet::Bullet(BulletType type, int x, int y) {
    base = Sprite::createWithTexture(assets::textures::test_bullet);
    root::scene->addChild(base, 1);

    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
                                        base->getContentSize().height * base->getScaleY()));
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setRotationEnable(false);
    pbody->setGravityEnable(false);
    pbody->setMass(FLT_MAX);
    pbody->setMoment(FLT_MAX);
    base->setPhysicsBody(pbody);

    pcontact_listener = EventListenerPhysicsContact::create();
    pcontact_listener->onContactBegin = CC_CALLBACK_1(Bullet::physics_contact, this);
    root::scene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pcontact_listener, base);
}

Bullet::~Bullet() {
    cleanup();
}

void Bullet::cleanup() {
    root::scene->removeChild(base, 1);
    pbody->autorelease();
    base->autorelease();
    root::scene->getEventDispatcher()->removeEventListener(pcontact_listener);
    pcontact_listener->autorelease();
}

void Bullet::schedule_removal() {
    to_be_removed = true;
}

bool Bullet::physics_contact(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        //schedule_removal();
    }

    return true;
}

void Bullet::update() {
    pbody->setVelocity(Vec2(0, 0));
    pbody->setAngularVelocity(0);
}

//-- end Bullet class --

END_BULLET_NS
END_ENTITIES_NS