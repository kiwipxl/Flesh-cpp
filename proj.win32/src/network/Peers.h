#ifndef _PEERS_H_
#define _PEERS_H_

#include <vector>
#include "sockets/SocketManager.h"

namespace peers {
    
    class Peer {

        public:
            Peer() { }

            int id;
            char* ip;
            u_short udp_port;
            Socket udp_sock;
            
            int bind_udp_sock();

        private:
            bool accepted = false;
    };
    
    extern std::vector<Peer*> peer_list;

    extern void peer_join(int id, char* ip, u_short port);
    extern void peer_leave(int id, char* ip, u_short port);
};

#endif