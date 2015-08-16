#ifndef _UNIT_H_
#define _UNIT_H_

/*
this class temporarily handles all player movement and other things, will be modified in future
*/

#include <vector>
#include <2d/CCSprite.h>
#include <physics/CCPhysicsContact.h>

#include "network/Peers.h"
#include "entities/EntityDefines.h"

BEGIN_ENTITIES_NS

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

        bool on_contact_run(cc::PhysicsContact&);
        void on_post_contact(cc::PhysicsContact&, const cc::PhysicsContactPostSolve&);
};

extern Unit* test_player;
extern std::vector<Unit*> units;

extern void test_peer_join(network::peers::Peer* peer);
extern void test_peer_movement(network::peers::Peer* peer, int x, int y, float rota);

extern void update_units();

END_ENTITIES_NS

#endif