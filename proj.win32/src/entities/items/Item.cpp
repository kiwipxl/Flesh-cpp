#include "entities/items/Item.h"

#include "assets/Textures.h"
#include "physics/Physics.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_ITEMS_NS

//private

//public
std::vector<ItemPtr> items;

bool on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        bool do_collide = false;
        for (int i = 0; i < items.size(); ++i) {
            if (items[i]->on_contact_run(contact)) do_collide = true;
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

ItemPtr spawn(ItemType _type, int _x, int _y) {
    ItemPtr c(new Item(_type, _x, _y));
    items.push_back(c);
    return c;
}

void update() {
    for (int n = 0; n < items.size(); ++n) {
        auto& b = items[n];
        if (!b) continue;
        if (!b->is_removal_scheduled()) b->update();
        if (b->is_removal_scheduled()) {
            items.erase(items.begin() + n, items.begin() + n + 1);
            --n;
        }
    }
}

//-- begin Item class --
Item::Item(ItemType _type, int _x, int _y) {
    type = _type;

    base = Sprite::createWithTexture(assets::textures::crate);
    base->setPosition(_x, _y);
    base->setScale(.2f);
    root::map_layer->addChild(base);

    pbody = PhysicsBody::createBox(Size(base->getContentSize().width * base->getScaleX(),
                                        base->getContentSize().height * base->getScaleY()), PHYSICSBODY_MATERIAL_DEFAULT);
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    pbody->setVelocityLimit(1000.0f);
    pbody->setMass(100.0f);
    base->setPhysicsBody(pbody);
}

void Item::update() {
    update_scheduler();
}

bool Item::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b && CHECK_AB_COLLIDE(base)) {
        return true;
    }

    return false;
}
//-- end Item class --

END_ITEMS_NS
END_ENTITIES_NS