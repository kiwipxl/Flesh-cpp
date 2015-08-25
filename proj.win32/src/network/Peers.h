#ifndef _PEERS_H_
#define _PEERS_H_

#include <vector>

#include "network/Defines.h"

BEGIN_NETWORK_NS

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
            sock::Socket* udp_sock = NULL;
            bool connected = false;

            int bind_udp_sock();

        private:
            bool accepted = false;
    };

    extern std::vector<Peer*> peer_list;

    extern void peer_join(int id, char* ip);
    extern void peer_leave(int id, char* ip);
    extern Peer* get_peer(int id);
    extern Peer* get_peer(sock::Socket& sock);
};

END_NETWORK_NS

#endif