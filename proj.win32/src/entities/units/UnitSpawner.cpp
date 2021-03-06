#include "entities/units/UnitSpawner.h"

#include "assets/Maps.h"
#include "entities/units/Unit.h"
#include "entities/units/components/PlayerMoveComponent.h"
#include "entities/units/components/BulletAimerComponent.h"
#include "entities/items/Item.h"
#include "input/KeyboardInput.h"
#include "gui/GameGUI.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_ENTITIES_NS
BEGIN_UNITS_NS

//public
std::vector<Unit*> all_units;
std::vector<UnitTeam*> unit_teams;
Unit* current_unit;
int current_unit_index;
int current_turn = 0;

void create_team_unit(int team_id) {
    auto unit = new Unit(unit_teams[team_id], (unit_teams[team_id]->num_members == 0) ? UNIT_TYPE_CAPTAIN : UNIT_TYPE_MINION);
    unit->inventory.push_back(items::weapon_c4);

    ++unit->team->num_members;

    unit->base->setPosition(assets::maps::test_terrain->spawn_points[all_units.size()]);
    all_units.push_back(unit);
}

void spawn_test_units() {
    unit_teams.push_back(new UnitTeam(0, Color3B(255, 100, 100)));
    unit_teams.push_back(new UnitTeam(1, Color3B(100, 100, 255)));

    for (int n = 0; n < 2; ++n) {
        create_team_unit(0);
    }
    for (int n = 0; n < 2; ++n) {
        create_team_unit(1);
    }
    select_current_unit();

    for (int n = 0; n < 2; ++n) {
        auto& i = items::spawn(items::ITEM_TYPE_CRATE, ((rand() / (float)RAND_MAX) * (4400 - 850)) + 850, 800);
    }

    gui::game::update_inventory();
}

void next_unit() {
    current_unit->remove_component<components::BulletAimerComponent>();
    current_unit->remove_component<components::PlayerMoveComponent>();

    ++current_unit_index;
    if (current_unit_index >= all_units.size()) current_unit_index = 0;

    select_current_unit();

    if (current_turn % 5 == 0) {
        auto& i = items::spawn(items::ITEM_TYPE_CRATE, ((rand() / (float)RAND_MAX) * (4400 - 850)) + 850, 800);
    }
    ++current_turn;

    gui::game::update_inventory();
}

void select_current_unit() {
    if (current_unit_index >= all_units.size()) current_unit_index = 0;
    current_unit = all_units[current_unit_index];
    current_unit->add_component<components::BulletAimerComponent>()->init();
    current_unit->add_component<components::PlayerMoveComponent>()->init();

    gui::game::reset_countdown();
}

void update_all_units() {
    if (input::key_down(EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(EventKeyboard::KeyCode::KEY_P)) {
        current_unit->schedule_removal();
    }

    for (int n = 0; n < all_units.size(); ++n) {
        if (!all_units[n]->is_removal_scheduled()) all_units[n]->update();
        if (all_units[n]->is_removal_scheduled()) {
            if (all_units.size() == 1) {
                root::switch_state(root::STATE_MENU);
                return;
            }

            gui::game::remove_ui_bar(all_units[n]);

            delete all_units[n];
            all_units.erase(all_units.begin() + n, all_units.begin() + n + 1);
            --n;

            if (!gui::game::countdown_paused) {
                select_current_unit();
                gui::game::reset_countdown();
            }
            if (current_unit_index >= all_units.size()) current_unit_index = 0;
            current_unit = all_units[current_unit_index];
            gui::game::sort_ui_bars();
            gui::game::update_inventory();

            break;
        }
    }
}

END_UNITS_NS
END_ENTITIES_NS