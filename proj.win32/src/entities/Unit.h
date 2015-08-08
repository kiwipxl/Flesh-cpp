#ifndef _UNIT_H_
#define _UNIT_H_

#include <vector>
#include <2d/CCSprite.h>

#include "network/Peers.h"

namespace entities {

    using namespace cocos2d;

    class Unit {

        public:
            Sprite* base;
            PhysicsBody* pbody;
            bool player_input = false;
            peers::Peer* peer;

            int dest_x;
            int dest_y;
            float dest_rota;

            Unit();

            void update();
    };

    extern Unit* test_player;
    extern std::vector<Unit*> units;

    extern void test_peer_join(peers::Peer* peer);
    extern void test_peer_movement(peers::Peer* peer, int x, int y, float rota);

    extern void update_units();
};

#endif