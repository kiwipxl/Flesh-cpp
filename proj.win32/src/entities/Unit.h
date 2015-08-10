#ifndef _UNIT_H_
#define _UNIT_H_

#include <vector>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsContact.h>

#include "network/Peers.h"

namespace entities {

    namespace cc = cocos2d;

    class Unit {

        public:
            Unit();

            cc::Sprite* base;
            cc::PhysicsBody* pbody;
            bool player_input = false;
            network::peers::Peer* peer;

            int dest_x;
            int dest_y;
            float dest_rota;

            void update();

        private:
            bool can_jump = false;
            bool facing_right = false;

            bool physics_contact(cc::PhysicsContact& contact);
    };

    extern Unit* test_player;
    extern std::vector<Unit*> units;

    extern void test_peer_join(network::peers::Peer* peer);
    extern void test_peer_movement(network::peers::Peer* peer, int x, int y, float rota);

    extern void update_units();
};

#endif