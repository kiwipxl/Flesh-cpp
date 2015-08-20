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

    ITEM_TYPE_CRATE, 
    ITEM_TYPE_GUN
};

class Item : public EntityScheduler {

public:
    Item(ItemType _type, int _x, int _y);

    cc::Sprite* base;
    cc::PhysicsBody* pbody;

    void update();
    bool on_contact_run(cc::PhysicsContact& contact);
    void on_contact_leave(cc::PhysicsContact& contact);

    ItemType get_type() { return type; }

private:
    ItemType type;
    cc::Label* info_label;
};

extern std::vector<ItemPtr> item_list;

extern void init();
extern void deinit();
extern ItemPtr spawn(ItemType _type, int _x, int _y);
extern void update();

END_ITEMS_NS
END_ENTITIES_NS

#endif