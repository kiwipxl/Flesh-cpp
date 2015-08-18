#ifndef _UNIT_H_
#define _UNIT_H_

/*
this class temporarily handles all player movement and other things, will be modified in future
*/

#include <vector>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsContact.h>

#include "network/Peers.h"
#include "entities/EntityDefines.h"
#include "entities/units/components/ComponentBase.h"

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

namespace cc = cocos2d;

enum UnitType {

    UNIT_TYPE_CAPTAIN, 
    UNIT_TYPE_MINION
};

enum UnitTeamType {

    UNIT_TEAM_BLUE,
    UNIT_TEAM_RED
};

namespace components {

    struct PlayerMoveComponent;
};

class Unit {

    public:
        Unit();
        ~Unit();

        cc::Sprite* base;
        cc::PhysicsBody* pbody;

        bool player_input = false;
        UnitType type;
        UnitTeamType team_type;

        void update();
        void despawn();

        void take_damage(float amount);

        void schedule_removal() { removal_scheduled = true; }
        bool is_scheduled_removal() { return removal_scheduled; }

        template <typename T> T*    add_component();
        template <typename T> void  remove_component();
        template <typename T> T*    get_component();

    private:
        bool removal_scheduled = false;
        std::vector<components::ComponentBase*> components;
        float damage = 0.0f;

        bool on_contact_run(cc::PhysicsContact&);
        //void on_post_contact(cc::PhysicsContact&, const cc::PhysicsContactPostSolve&);
};

extern std::vector<Unit*> all_units;

extern void test_peer_join(network::peers::Peer* peer);
extern void test_peer_movement(network::peers::Peer* peer, int x, int y, float rota);

extern void update_all_units();

extern Unit* current_unit;
extern int current_unit_index;

extern void next_unit();
extern void select_current_unit();

END_UNITS_NS
END_ENTITIES_NS

#endif