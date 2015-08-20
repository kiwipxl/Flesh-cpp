#ifndef _ITEM_H_
#define _ITEM_H_

#include <vector>

#include <2d/CCSprite.h>
#include <physics/CCPhysicsBody.h>

#include "entities/EntityDefines.h"
#include "entities/EntityScheduler.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS
BEGIN_ITEMS_NS

enum ItemType {

    ITEM_TYPE_NONE, 
    ITEM_TYPE_CRATE, 
    ITEM_TYPE_GUN
};

class Item : public EntityScheduler {

public:
    Item(ItemType _type, int _gun_type, int _x, int _y);
    ~Item();

    cc::Sprite* base;
    cc::PhysicsBody* pbody;
    int gun_type;       //temporary

    void update();
    void cleanup();
    bool on_contact_run(cc::PhysicsContact& contact);
    void on_contact_leave(cc::PhysicsContact& contact);
    void take_damage(float amount);

    ItemType get_type() { return type; }

private:
    ItemType type = ITEM_TYPE_NONE;
    cc::Label* info_label;
    int health = 5.5f;
};

extern std::vector<ItemPtr> item_list;

extern void init();
extern void deinit();
extern ItemPtr spawn(ItemType _type, int _gun_type, int _x, int _y);
extern void update();

END_ITEMS_NS
END_ENTITIES_NS

#endif