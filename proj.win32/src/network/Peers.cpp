#include "network/Peers.h"

#include "debug/Errors.h"
#include "debug/Logger.h"
#include "network/sockets/Socket.h"

BEGIN_NETWORK_NS

namespace peers {

    using debug::fresult;

    std::vector<Peer*> peer_list;

    int Peer::bind_udp_sock() {
        if (udp_sock != NULL) return -1;

        udp_sock = new sock::Socket(sock::PROTO_UDP);
        if ((fresult = udp_sock->s_create()) > 0) { return fresult; }
        if ((fresult = udp_sock->s_bind("0.0.0.0", 0)) > 0) { return fresult; }
        udp_recv_port = udp_sock->get_binded_port();

        return NO_ERROR;
    }

    void peer_join(int id, char* ip) {
        Peer* p = new Peer();
        p->id = id;
        p->ip = new char[strlen(ip)];
        strcpy(p->ip, ip);

        peer_list.push_back(p);
        if (p->bind_udp_sock() == NO_ERROR) {
            log_info << "udp peer socket successfully binded (peer_id: " << id << ", ip: " << ip << ", binded_port: " << p->udp_recv_port;
            //msg::send(sock::tcp_serv_sock, msg::MsgStream() << _MID->SEND_UDP_PEER_BIND_PORT_SUCCESS << p->id << p->ip << p->udp_recv_port);
        }else {
            log_warning << "udp peer socket binnd failed (peer_id: " << id << ", ip: " << ip << ")";
            //msg::send(sock::tcp_serv_sock, msg::MsgStream() << _MID->SEND_UDP_PEER_BIND_PORT_FAILED << p->id << p->ip);
        }
    }

    void peer_leave(int id, char* ip) {

    }

    Peer* get_peer(int id) {
        for (int n = 0; n < peer_list.size(); ++n) {
            if (peer_list[n]->id == id) {
                return peer_list[n];
            }
        }
        return NULL;
    }

    Peer* get_peer(sock::Socket& sock) {
        for (int n = 0; n < peer_list.size(); ++n) {
            if (peer_list[n]->udp_sock == &sock) {
                return peer_list[n];
            }
        }
        return NULL;
    }
};

END_NETWORK_NS