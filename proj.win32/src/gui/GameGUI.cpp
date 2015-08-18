#include "gui/GameGUI.h"

#include "assets/Textures.h"
#include "entities/units/Unit.h"
#include "entities/units/UnitSpawner.h"
#include "StateManager.h"

using namespace cocos2d;

BEGIN_GUI_NS
BEGIN_GAME_NS

//private

//public
std::vector<UnitUIBar*> unit_ui_bars;

//-- begin UnitUIBar class --
UnitUIBar::UnitUIBar(entities::units::Unit* _unit) {
    unit = _unit;

    bar = Sprite::createWithTexture(unit->type == entities::units::UNIT_TYPE_MINION ? assets::textures::minion_health_bar : 
                                    assets::textures::captain_health_bar);
    bar->setAnchorPoint(Vec2(0, 0));
    root::ui_layer->addChild(bar, 1);
}
//-- end UnitUIBar class --

void init_ui_bars() {
    for (auto& u : entities::units::all_units) {
        auto ui_bar = new UnitUIBar(u);
        unit_ui_bars.push_back(ui_bar);
    }
    sort_ui_bars();
}

void sort_ui_bars() {
    int height = assets::textures::captain_health_bar->getContentSize().height;
    Vec2 pos{ 0, root::scene->screen_size.height };
    for (auto& b : unit_ui_bars) {
        pos.y -= height + 4;
        b->bar->setColor(b->unit->team->get_colour());
        b->bar->setPosition(pos);
    }
}

void update() {

}

END_GAME_NS
END_GUI_NS