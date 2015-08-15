#include "entities/Bullet.h"

#include "assets/Textures.h"
#include "entities/BulletLogic.h"
#include "debug/Logger.h"
#include "SceneManager.h"
#include "StateManager.h"
#include "states/Game.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

//private
std::vector<BulletPtr> bullets;

bool on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        for (int n = 0; n < bullets.size(); ++n) {
            if (a == bullets[n]->base || b == bullets[n]->base) {
                if (b == states::game::terrain->base || a == states::game::terrain->base) {
                    bullets[n]->on_contact_run(contact);
                }
            }
        }
    }

    return false;
}

//public

void init() {
    physics::add_on_contact_run(on_contact_run, NULL);
}

void deinit() {
    physics::remove_on_contact_run(on_contact_run);
}

BulletPtr create_bullet(int x, int y) {
    BulletPtr b(new Bullet(x, y));
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

Bullet::Bullet(int x, int y) {
    base = Sprite::createWithTexture(assets::textures::test_bullet);
    base->setPosition(x, y);
    root::scene->addChild(base, 1);

    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
        base->getContentSize().height * base->getScaleY()));
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setRotationEnable(false);
    base->setPhysicsBody(pbody);

    add_logic_decay(2.0f);
}

Bullet::~Bullet() {
    cleanup();
}

void Bullet::cleanup() {
    for (int n = 0; n < logic_list.size(); ++n) {
        delete logic_list[n];
    }
    logic_list.clear();
    root::scene->removeChild(base, 1);
}

void Bullet::schedule_removal() {
    to_be_removed = true;
}

bool Bullet::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b && (a == states::game::terrain->base || b == states::game::terrain->base)) {
        auto bullet_explosion = ParticleSystemQuad::create(assets::particles::bullet_explosion_name);
        bullet_explosion->setPosition(base->getPosition());
        bullet_explosion->setScale(.325f);
        root::scene->addChild(bullet_explosion, 1);

        schedule_removal();
    }

    return true;
}

void Bullet::update() {
    for (int n = 0; n < logic_list.size(); ++n) {
        logic_list[n]->update();
    }
}

void Bullet::add_logic_decay(float decay_after_seconds) {
    type = BULLET_TYPE_DECAY;
    logic_list.push_back(new BulletLogicDecay(this, decay_after_seconds));
}

void Bullet::add_logic_test(float angle, float power) {
    type = BULLET_TYPE_TEST;
    logic_list.push_back(new BulletLogicTest(this, angle, power));
}

void Bullet::add_logic_test2(float angle, float power) {
    type = BULLET_TYPE_TEST2;
    logic_list.push_back(new BulletLogicTest2(this, angle, power));
}

//-- end Bullet class --

END_BULLET_NS
END_ENTITIES_NS