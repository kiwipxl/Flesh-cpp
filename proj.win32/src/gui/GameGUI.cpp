#include "gui/GameGUI.h"

#include "assets/Assets.h"
#include "assets/Textures.h"
#include "entities/bullets/Bullet.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_GUI_NS
BEGIN_GAME_NS

//private
Label* turn_time_label;
clock_t countdown_start;
Label* power_label;

//public
std::vector<UnitUIBar*> unit_ui_bars;
int starting_countdown_seconds = 10;
int current_countdown_seconds;

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
    current_countdown_seconds = starting_countdown_seconds;
    countdown_start = clock();
}

void set_countdown_to(float seconds) {
    current_countdown_seconds = seconds;
    countdown_start = clock();
}

void set_power_text(float power) {
    power_label->setString(sstream_cstr("power: " << power << "x"));
}

void wait_for_bullet(entities::bullets::BulletGroupPtr bullet) {

}

void init() {
    turn_time_label = Label::createWithBMFont("fonts/felt.fnt", "");
    turn_time_label->setPosition(root::scene->screen_size.width / 2.0f, root::scene->screen_size.height - 40);
    turn_time_label->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
    root::ui_layer->addChild(turn_time_label, 1);
    countdown_start = clock();
    current_countdown_seconds = starting_countdown_seconds;

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