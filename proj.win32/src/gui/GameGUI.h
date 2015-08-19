#ifndef _GAME_GUI_H_
#define _GAME_GUI_H_

#include <vector>

#include <2d/CCSprite.h>

#include "gui/GUIDefines.h"

namespace cc = cocos2d;

//forward declares begin
namespace entities {

    namespace units {

        class Unit;
    };
};
//forward declares end

BEGIN_GUI_NS
BEGIN_GAME_NS

class UnitUIBar {

public:
    UnitUIBar(entities::units::Unit* _unit);
    ~UnitUIBar();

    entities::units::Unit* unit;
    cc::Sprite* bar;
    
    bool is_created() { return created; }
    int get_height() { return height; }

private:
    bool created = false;
    int height;
};

extern std::vector<UnitUIBar*> unit_ui_bars;

extern void init_ui_bars();
extern void sort_ui_bars();
extern void remove_ui_bar(entities::units::Unit* unit);
extern void update();

END_GAME_NS
END_GUI_NS

#endif