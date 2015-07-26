#ifndef _PEERS_H_
#define _PEERS_H_

#include <vector>
#include "sockets/SocketManager.h"

namespace peers {
    
    class Peer {

        public:
            Peer() { }
            ~Peer() {
                if (ip) {
                    delete[] ip;
                }
            }

            int id = -1;
            char* ip = NULL;
            u_short udp_send_port = 0;
            u_short udp_recv_port = 0;
            Socket* udp_sock = NULL;

            int bind_udp_sock();

        private:
            bool accepted = false;
    };
    
    extern std::vector<Peer*> peer_list;

    extern void peer_join(int id, char* ip);
    extern void peer_leave(int id, char* ip);
    extern Peer* get_peer(int id);
    extern Peer* get_peer(Socket& sock);
};

#endif