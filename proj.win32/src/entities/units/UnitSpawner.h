#ifndef _UNIT_SPAWNER_H_
#define _UNIT_SPAWNER_H_

#include <vector>

#include "entities/EntityDefines.h"

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

class Unit;

class UnitTeam {

public:
    UnitTeam(int _team_id) : team_id(team_id) {

    }

    //returns the number of members in the team
    int get_num_members() { return num_members; }

private:
    //the id (or index) in unit_teams vector
    int team_id;
    //the number of members in the team
    int num_members = 0;
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