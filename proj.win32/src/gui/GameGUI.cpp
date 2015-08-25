#include "gui/GameGUI.h"

#include "assets/Assets.h"
#include "assets/Textures.h"
#include "entities/bullets/Bullet.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "entities/units/components/BulletAimerComponent.h"
#include "entities/items/Weapon.h"
#include "gui/Button.h"
#include "StateManager.h"
#include "utility/String.h"

using namespace cocos2d;

BEGIN_GUI_NS
BEGIN_GAME_NS

//private
Label* turn_time_label;
clock_t countdown_start;
Label* power_label;
Sprite* footer;
gui::ButtonPtr footer_fire_button;
std::vector<gui::ButtonPtr> inventory_buttons;

//public
std::vector<UnitUIBar*> unit_ui_bars;
int starting_countdown_seconds = 20;
int current_countdown_seconds;
extern bool countdown_paused = false;

//-- begin UnitUIBar class --
UnitUIBar::UnitUIBar(entities::units::Unit* _unit) {
    unit = _unit;

    bar = Sprite::createWithTexture(unit->type == entities::units::UNIT_TYPE_MINION ? assets::textures::minion_health_bar : 
                                    assets::textures::captain_health_bar);
    height = bar->getContentSize().height;
    bar->setAnchorPoint(Vec2(0, 0));
    root::ui_layer->addChild(bar, 1);
}

UnitUIBar::~UnitUIBar() {
    root::ui_layer->removeChild(bar);
}
//-- end UnitUIBar class --

void reset_countdown() {
    if (turn_time_label) turn_time_label->setVisible(true);
    countdown_paused = false;
    current_countdown_seconds = starting_countdown_seconds;
    countdown_start = clock();
}

void set_countdown_to(float seconds) {
    current_countdown_seconds = seconds;
    countdown_start = clock();
}

void set_power_text(float power) {
    if (power_label) power_label->setString(sstream_cstr("power: " << power << "x"));
}

void wait_for_bullet(entities::bullets::BulletGroupPtr bullet) {
    turn_time_label->setVisible(false);
    countdown_paused = true;
}

void init_ui_bars() {
    turn_time_label = Label::createWithBMFont("fonts/felt.fnt", "");
    turn_time_label->setPosition(root::scene->screen_size.width / 2.0f, root::scene->screen_size.height - 40);
    turn_time_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    root::ui_layer->addChild(turn_time_label, 1);
    reset_countdown();

    power_label = Label::createWithBMFont("fonts/felt.fnt", "");
    power_label->setPosition(root::scene->screen_size.width / 2.0f, 40);
    power_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    root::ui_layer->addChild(power_label, 1);

    for (auto& u : entities::units::all_units) {
        auto ui_bar = new UnitUIBar(u);
        unit_ui_bars.push_back(ui_bar);
    }
    sort_ui_bars();
}

void init_footer() {
    footer = Sprite::createWithTexture(assets::textures::footer);
    footer->setScale(root::scene->screen_size.width / footer->getContentSize().width);
    footer->setAnchorPoint(Vec2(0.0f, 0.0f));
    root::ui_layer->addChild(footer);

    footer_fire_button = gui::create_button(750, 30);
    footer_fire_button->set_idle_texture(assets::textures::footer_fire_button);
    footer_fire_button->base->setScale(.4f);
    root::ui_layer->addChild(footer_fire_button->base, 10);
    footer_fire_button->set_on_click_callback([]() {
        if (entities::units::current_unit) {
            auto& t = entities::units::current_unit->get_component<entities::units::components::BulletAimerComponent>();
            if (t) {
                t->fire();
            }
        }
    });

    for (int i = 0; i < 2; ++i) {
        auto& b = gui::create_button(40.0f + (i * 70.0f), 40.0f);
        b->set_idle_texture(assets::textures::inventory_empty);
        root::ui_layer->addChild(b->base, 10);
        inventory_buttons.push_back(b);

        b->set_on_click_callback([b]() {
            if (b->get_idle_texture() == assets::textures::inventory_empty) return;

            auto e = entities::units::current_unit;
            if (e) {
                auto c = e->get_component<entities::units::components::BulletAimerComponent>();
                if (c) {
                    auto w = entities::items::weapon_none;
                    if (b->get_idle_texture() == assets::textures::inventory_c4) w = entities::items::weapon_c4;
                    if (b->get_idle_texture() == assets::textures::inventory_fireworks_gun) w = entities::items::weapon_flame_fireworks;
                    c->switch_weapon(w);
                    c->begin_aiming();
                }
            }
        });
    }
}

void update_inventory() {
    int n = 0;
    for (auto i : inventory_buttons) {
        entities::items::Weapon* w = entities::items::weapon_none;
        if (n <= entities::units::current_unit->inventory.size() - 1) w = entities::units::current_unit->inventory[n];
        auto a = assets::textures::inventory_empty;
        if (w == entities::items::weapon_c4) a = assets::textures::inventory_c4;
        else if (w == entities::items::weapon_flame_fireworks) a = assets::textures::inventory_fireworks_gun;
        i->set_idle_texture(a);
        ++n;
    }
}

void sort_ui_bars() {
    Vec2 pos{ 0, root::scene->screen_size.height };
    for (auto& b : unit_ui_bars) {
        pos.y -= b->get_height() + 4;
        b->bar->setColor(b->unit->team->get_colour());
        b->bar->setScaleX(b->unit->get_health() / b->unit->get_max_health());
        b->bar->setPosition(pos);
    }
}

void remove_ui_bar(entities::units::Unit* unit) {
    for (int n = 0; n < unit_ui_bars.size(); ++n) {
        if (unit_ui_bars[n]->unit == unit) {
            delete unit_ui_bars[n];
            unit_ui_bars.erase(unit_ui_bars.begin() + n, unit_ui_bars.begin() + n + 1);
            --n;
        }
    }
}

void update() {
    if (countdown_paused) return;

    float t = (current_countdown_seconds * 1000) - (clock() - countdown_start);
    if (t <= 0) {
        reset_countdown();
        entities::units::next_unit();
        return;
    }
    int seconds = t / 1000.0f;
    int ms = clampf(fmod(t, 1000.0f) / 10.0f, 0, 99);
    turn_time_label->setString(sstream_str(seconds << ":" << ms));
}

END_GAME_NS
END_GUI_NS