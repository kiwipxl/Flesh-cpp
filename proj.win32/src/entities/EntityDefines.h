#ifndef _ENTITY_DEFINES_H_
#define _ENTITY_DEFINES_H_

/*
contains all namespace declarations for all entity modules as well as forward declares and any other
relevant defines
*/

#define BEGIN_ENTITIES_NS           namespace entities {
#define END_ENTITIES_NS             };

#define BEGIN_UNITS_NS              namespace units {
#define END_UNITS_NS                };

#define BEGIN_COMPONENTS_NS         namespace components {
#define END_COMPONENTS_NS           };

#define BEGIN_BULLETS_NS            namespace bullets {
#define END_BULLETS_NS              };

#define BEGIN_ITEMS_NS              namespace items {
#define END_ITEMS_NS                };

//forward declarations begin
BEGIN_ENTITIES_NS
    BEGIN_BULLETS_NS
        class Bullet;
        typedef std::shared_ptr<Bullet> BulletPtr;

        class BulletGroup;
        typedef std::shared_ptr<BulletGroup> BulletGroupPtr;
    END_BULLETS_NS

    BEGIN_UNITS_NS
        class Unit;
        struct UnitTeam;

        BEGIN_COMPONENTS_NS
            struct PlayerMoveComponent;
            struct BulletAimerComponent;
            struct ColliderComponent;

            typedef std::shared_ptr<PlayerMoveComponent> PlayerMoveComponentPtr;
            typedef std::shared_ptr<BulletAimerComponent> BulletAimerComponentPtr;
            typedef std::shared_ptr<ColliderComponent> ColliderComponentPtr;
        END_COMPONENTS_NS
    END_UNITS_NS

    BEGIN_ITEMS_NS
        class Item;
        typedef std::shared_ptr<Item> ItemPtr;
    END_ITEMS_NS
END_ENTITIES_NS

namespace cocos2d {

    class PhysicsContact;
    class Label;
    class Texture2D;
};
//forward declarations end

#endif