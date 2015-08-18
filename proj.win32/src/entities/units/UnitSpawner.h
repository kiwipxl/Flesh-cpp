#ifndef _UNIT_SPAWNER_H_
#define _UNIT_SPAWNER_H_

#include <vector>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"

namespace cc = cocos2d;

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

class Unit;

class UnitTeam {

public:
    UnitTeam(int _team_id, cc::Color3B _colour) : team_id(team_id), colour(_colour) { }

    //the number of members in the team
    int num_members = 0;

    cc::Color3B get_colour() { return colour; }

private:
    //the id (or index) in unit_teams vector
    int team_id;
    //team colour
    cc::Color3B colour;
};

extern std::vector<Unit*> all_units;
extern std::vector<UnitTeam*> unit_teams;
extern Unit* current_unit;
extern int current_unit_index;

extern void spawn_test_units();
extern void update_all_units();
extern void next_unit();
extern void select_current_unit();

END_UNITS_NS
END_ENTITIES_NS

#endif