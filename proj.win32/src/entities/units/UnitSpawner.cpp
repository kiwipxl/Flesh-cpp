#include "entities/units/UnitSpawner.h"

#include "assets/Maps.h"
#include "entities/units/Unit.h"
#include "entities/units/components/PlayerMoveComponent.h"
#include "entities/units/components/BulletAimerComponent.h"
#include "gui/GameGUI.h"

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

//public
std::vector<Unit*> all_units;
std::vector<UnitTeam*> unit_teams;
Unit* current_unit;
int current_unit_index;

void create_team_unit(int team_id) {
    auto unit = new Unit();
    unit->team = unit_teams[team_id];
    unit->base->setPosition(assets::maps::test_terrain->spawn_points[all_units.size()]);
    all_units.push_back(unit);
}

void spawn_test_units() {
    unit_teams.push_back(new UnitTeam(0));
    unit_teams.push_back(new UnitTeam(1));

    for (int n = 0; n < 2; ++n) {
        create_team_unit(0);
    }
    for (int n = 0; n < 2; ++n) {
        create_team_unit(1);
    }
    select_current_unit();
    current_unit->add_component<components::BulletAimerComponent>()->init();
    current_unit->add_component<components::PlayerMoveComponent>()->init();

    gui::game::init_ui_bars();
}

void next_unit() {
    current_unit->player_input = false;
    ++current_unit_index;
    if (current_unit_index >= all_units.size()) {
        current_unit_index = 0;
    }
    current_unit = all_units[current_unit_index];
    current_unit->player_input = true;
}

void select_current_unit() {
    current_unit = all_units[current_unit_index];
    current_unit->player_input = true;
}

void update_all_units() {
    for (int n = 0; n < all_units.size(); ++n) {
        if (!all_units[n]->is_scheduled_removal()) all_units[n]->update();
        if (all_units[n]->is_scheduled_removal()) {
            delete all_units[n];
            all_units.erase(all_units.begin() + n, all_units.begin() + n + 1);
            --n;
            select_current_unit();
        }
    }
}

END_UNITS_NS
END_ENTITIES_NS