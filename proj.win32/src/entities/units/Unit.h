#ifndef _UNIT_H_
#define _UNIT_H_

/*
this class temporarily handles all player movement and other things, will be modified in future
*/

#include <vector>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsContact.h>

#include "entities/EntityDefines.h"
#include "entities/EntityScheduler.h"
#include "entities/units/components/ComponentBase.h"

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

namespace cc = cocos2d;

enum UnitType {

    UNIT_TYPE_CAPTAIN,
    UNIT_TYPE_MINION
};

class Unit : public EntityScheduler {

    public:
        Unit(UnitTeam* _team, UnitType _type);
        ~Unit();

        cc::Sprite* base;
        cc::Sprite* shadow;
        cc::PhysicsBody* pbody;

        UnitType type;
        UnitTeam* team;

        void update();

        void take_damage(float damage);

        float get_health() { return health; }
        float get_max_health() { return max_health; }

        template <typename T> std::shared_ptr<T>    add_component();
        template <typename T> void  remove_component();
        template <typename T> std::shared_ptr<T>    get_component();

    private:
        std::vector<std::shared_ptr<components::ComponentBase>> components;
        float health;
        float max_health;

        bool on_contact_run(cc::PhysicsContact&);
        //void on_post_contact(cc::PhysicsContact&, const cc::PhysicsContactPostSolve&);
};

END_UNITS_NS
END_ENTITIES_NS

#endif