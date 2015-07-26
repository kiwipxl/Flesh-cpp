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
            void set_port(u_short port) { udp_port = port; }

        private:
            bool accepted = false;
    };
    
    extern std::vector<Peer*> peer_list;

    extern void peer_join(int id, char* ip);
    extern void peer_leave(int id, char* ip);
    extern Peer* get_peer(int id, char* ip);
};

#endif