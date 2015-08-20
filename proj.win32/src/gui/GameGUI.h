#ifndef _GAME_GUI_H_
#define _GAME_GUI_H_

#include <vector>

#include <2d/CCSprite.h>

#include "entities/EntityDefines.h"
#include "gui/GUIDefines.h"
#include "gui/Button.h"

namespace cc = cocos2d;

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

extern int starting_countdown_seconds;
extern int current_countdown_seconds;
extern bool countdown_paused;

extern void set_countdown_to(float seconds);
extern void set_power_text(float power);
extern void reset_countdown();
extern void wait_for_bullet(entities::bullets::BulletGroupPtr bullet);

extern void init();
extern void sort_ui_bars();
extern void remove_ui_bar(entities::units::Unit* unit);
extern void update();

END_GAME_NS
END_GUI_NS

#endif