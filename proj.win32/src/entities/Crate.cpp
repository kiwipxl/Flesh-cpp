#include "entities/Crate.h"

#include "assets/Textures.h"
#include "physics/Physics.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_CRATES_NS

//private

//public
std::vector<Crate*> crates;

bool on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        bool do_collide = false;
        for (int i = 0; i < crates.size(); ++i) {
            if (crates[i]->on_contact_run(contact)) do_collide = true;
        }
        return do_collide;
    }

    return false;
}

void init() {
    physics::add_on_contact_run(on_contact_run, NULL);
}

void deinit() {
    physics::remove_on_contact_run(on_contact_run);
}

void spawn(int _x, int _y) {
    Crate* c = new Crate(_x, _y);
    crates.push_back(c);
}

void update() {
    for (int n = 0; n < crates.size(); ++n) {
        auto& b = crates[n];
        if (!b) continue;
        if (!b->is_removal_scheduled()) b->update();
        if (b->is_removal_scheduled()) {
            delete crates[n];
            crates.erase(crates.begin() + n, crates.begin() + n + 1);
            --n;
        }
    }
}

//-- begin Crate class --
Crate::Crate(int _x, int _y) {
    base = Sprite::createWithTexture(assets::textures::crate);
    base->setPosition(_x, _y);
    base->setScale(.2f);
    root::map_layer->addChild(base);

    PhysicsMaterial mat;
    mat.density = 5.0f;
    mat.friction = 1.0f;
    mat.restitution = 1.0f;
    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
                                        base->getContentSize().height * base->getScaleY()), mat);
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setVelocityLimit(1000.0f);
    pbody->setMass(100.0f);
    base->setPhysicsBody(pbody);
}

void Crate::update() {
    update_scheduler();
}

bool Crate::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b && CHECK_AB_COLLIDE(base)) {
        return true;
    }

    return false;
}
//-- end Crate class --

END_CRATES_NS
END_ENTITIES_NS