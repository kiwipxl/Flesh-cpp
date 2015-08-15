#include "entities/Bullet.h"

#include "assets/Textures.h"
#include "assets/Particles.h"
#include "debug/Logger.h"
#include "SceneManager.h"
#include "StateManager.h"
#include "states/Game.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_BULLET_NS

//private
std::vector<BulletPtr> bullets;

struct BulletDataBase {

    int timer = 0;
    std::function<void()> update_callback = nullptr;

    virtual ~BulletDataBase() {

    }

    virtual void update() = 0;
};

struct BulletDataTest : public BulletDataBase, public Bullet {

    bool gen_explosion = false;

    BulletDataTest(float angle, float power, const Bullet* bullet_ref) : Bullet(*bullet_ref) {
        type = BULLET_TYPE_TEST;
        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        pbody->applyImpulse(Vec2(force_x, force_y));
    }

    virtual ~BulletDataTest() {

    }

    virtual void update() {
        if (gen_explosion) return;
        if (++timer >= 40) {
            gen_explosion = true;
            for (int n = 0; n < 8; ++n) {
                auto b = create_bullet(base->getPositionX(), base->getPositionY());
                float angle = ((rand() / (float)RAND_MAX) * 90.0f) + 45.0f;
                b->add_btype_test2(angle, ((rand() / (float)RAND_MAX) * .2f) + .5f);
            }
            auto bullet_explosion = ParticleSystemQuad::create("Ring.plist");
            bullet_explosion->setPosition(base->getPosition());
            bullet_explosion->setScale(.8f);
            root::scene->addChild(bullet_explosion, 1);
        }
    }
};

struct BulletDataTest2 : public BulletDataBase, public Bullet {

    bool gen_explosion = false;

    BulletDataTest2(float angle, float power, const Bullet* bullet_ref) : Bullet(*bullet_ref) {
        type = BULLET_TYPE_TEST;
        float force_x = cos(angle / (180.0f / M_PI)) * 100000.0f * power;
        float force_y = sin(angle / (180.0f / M_PI)) * 100000.0f * power;
        pbody->applyImpulse(Vec2(force_x, force_y));
    }

    virtual ~BulletDataTest2() {

    }

    virtual void update() {

    }
};

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
}

Bullet::~Bullet() {
    cleanup();
}

void Bullet::cleanup() {
    root::scene->removeChild(base, 1);
    for (int n = 0; n < data_types.size(); ++n) {
        delete data_types[n];
    }
    data_types.clear();
    physics::remove_on_contact_run(this);
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
    for (int n = 0; n < data_types.size(); ++n) {
        data_types[n]->update();
    }
}

void Bullet::add_btype_test(float angle, float power) {
    data_types.push_back(new BulletDataTest(angle, power, this));
}

void Bullet::add_btype_test2(float angle, float power) {
    data_types.push_back(new BulletDataTest2(angle, power, this));
}

//-- end Bullet class --

END_BULLET_NS
END_ENTITIES_NS