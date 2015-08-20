#include "entities/items/Item.h"

#include "assets/Textures.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "physics/Physics.h"
#include "states/Game.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_ITEMS_NS

//private

//public
std::vector<ItemPtr> item_list;

bool on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        bool do_collide = false;
        for (int i = 0; i < item_list.size(); ++i) {
            if (!item_list[i]) continue;
            if (item_list[i]->on_contact_run(contact)) do_collide = true;
        }
        return do_collide;
    }

    return false;
}

void on_contact_leave(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b) {
        for (int i = 0; i < item_list.size(); ++i) {
            if (!item_list[i]) continue;
            item_list[i]->on_contact_leave(contact);
        }
    }
}

void init() {
    physics::add_on_contact_run(on_contact_run, NULL);
    physics::add_on_contact_leave(on_contact_leave, NULL);
}

void deinit() {
    physics::remove_on_contact_run(on_contact_run);
    physics::remove_on_contact_leave(on_contact_leave);
}

ItemPtr spawn(ItemType _type, int _gun_type, int _x, int _y) {
    ItemPtr c(new Item(_type, _gun_type, _x, _y));
    item_list.push_back(c);
    return c;
}

void update() {
    for (int n = 0; n < item_list.size(); ++n) {
        auto& b = item_list[n];
        if (!b) continue;
        if (!b->is_removal_scheduled()) b->update();
        if (b->is_removal_scheduled()) {
            item_list.erase(item_list.begin() + n, item_list.begin() + n + 1);
            --n;
        }
    }
}

//-- begin Item class --
Item::Item(ItemType _type, int _gun_type, int _x, int _y) {
    type = _type;
    gun_type = _gun_type;

    base = Sprite::create();
    base->setPosition(_x, _y);
    root::map_layer->addChild(base);

    switch (type) {
    case ITEM_TYPE_CRATE:
        base->setTexture(assets::textures::crate);
        base->setScale(.2f);
        break;
    case ITEM_TYPE_GUN:
        //very temporary if statement
        if (gun_type == 0) {
            base->setTexture(assets::textures::laser_machine_gun);
            base->setScale(.2f);
        }else if (gun_type == 1) {
            base->setTexture(assets::textures::c4);
            base->setScale(.55f);
        }
        info_label = Label::create("0", "fonts/Roboto-Light.ttf", 20.0f);
        root::map_layer->addChild(info_label);
        info_label->setVisible(false);
        break;
    }
    Size s = base->getTexture()->getContentSize();
    base->setTextureRect(Rect(0, 0, s.width, s.height));

    pbody = PhysicsBody::createBox(Size(s.width * base->getScaleX(), s.height * base->getScaleY()), PHYSICSBODY_MATERIAL_DEFAULT);
    pbody->setCollisionBitmask(1);
    pbody->setContactTestBitmask(true);
    //pbody->setRotationEnable(false);
    pbody->setVelocityLimit(1000.0f);
    pbody->setMass(100.0f);
    base->setPhysicsBody(pbody);
}

Item::~Item() {
    root::map_layer->removeChild(base);
    switch (type) {
    case ITEM_TYPE_CRATE:
        break;
    case ITEM_TYPE_GUN:
        root::map_layer->removeChild(info_label);
        break;
    }
}

void Item::update() {
    update_scheduler();

    switch (type) {
    case ITEM_TYPE_CRATE:
        break;
    case ITEM_TYPE_GUN:
        info_label->setPosition(base->getPositionX(), base->getPositionY() + 40);
        break;
    }
}

void Item::take_damage(float amount) {
    health -= amount;
    if (health <= 0) {
        if (type == ITEM_TYPE_CRATE) {
            for (int n = 0; n < 3; ++n) {
                auto & i = spawn(ITEM_TYPE_GUN, (rand() / (float)RAND_MAX) * 2.0f, base->getPositionX(), base->getPositionY());
                i->pbody->setVelocity(Vec2(((rand() / (float)RAND_MAX) * 250.0f) - 125.0f, ((rand() / (float)RAND_MAX) * 400.0f) - 200.0f));
            }
        }
        schedule_removal();
    }
}

bool Item::on_contact_run(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b && CHECK_AB_COLLIDE(base)) {
        switch (type) {
        case ITEM_TYPE_CRATE:
            return true;
        case ITEM_TYPE_GUN:
            for (auto& u : units::all_units) {
                if (CHECK_AB_COLLIDE(u->base)) {
                    info_label->setVisible(true);
                    info_label->setString("press X to pickup");
                    return false;
                }
            }
            return true;
        }
    }

    return false;
}

void Item::on_contact_leave(PhysicsContact& contact) {
    auto a = contact.getShapeA()->getBody()->getNode();
    auto b = contact.getShapeB()->getBody()->getNode();

    if (a && b && CHECK_AB_COLLIDE(base)) {
        switch (type) {
        case ITEM_TYPE_CRATE:
            break;
        case ITEM_TYPE_GUN:
            for (auto& u : units::all_units) {
                if (CHECK_AB_COLLIDE(u->base)) {
                    info_label->setVisible(false);
                    return;
                }
            }
            break;
        }
    }
}
//-- end Item class --

END_ITEMS_NS
END_ENTITIES_NS