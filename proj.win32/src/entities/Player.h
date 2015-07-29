#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "../SceneManager.h"

namespace entity {

    class Player {

        public:
            cc::Sprite* base;
            cc::PhysicsBody* pbody;

            Player();

            void update();
    };
};

#endif