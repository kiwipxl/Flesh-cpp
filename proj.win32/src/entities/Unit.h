#ifndef _UNIT_H_
#define _UNIT_H_

#include "../SceneManager.h"

namespace entity {

    class Unit {

        public:
            cc::Sprite* base;
            cc::PhysicsBody* pbody;
            bool player_input = false;

            Unit();

            void update();
    };

    extern Unit* test_player;
};

#endif