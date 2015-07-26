#include "Peers.h"
#include "../debug/Errors.h"

using peers::Peer;
using err::fresult;

std::vector<Peer*> peers::peer_list;

int Peer::bind_udp_sock() {
    if (udp_sock != NULL) return -1;

    udp_sock = new Socket(PROTO_UDP);
    if ((fresult = udp_sock->s_create()) > 0) { return fresult; }
    if ((fresult = udp_sock->s_bind(ip, 0)) > 0) { return fresult; }
    udp_recv_port = udp_sock->get_binded_port();

    return NO_ERROR;
}

void peers::peer_join(int id, char* ip) {
    Peer* p = new Peer();
    p->id = id;
    p->ip = new char[strlen(ip)];
    strcpy(p->ip, ip);

    peer_list.push_back(p);
    if (p->bind_udp_sock() == NO_ERROR) {
        msg::send(sock::tcp_serv_sock, msg::ByteStream() << _MID->SEND_UDP_PEER_BIND_PORT_SUCCESS << p->id << p->ip << p->udp_recv_port);
    }else {
        msg::send(sock::tcp_serv_sock, msg::ByteStream() << _MID->SEND_UDP_PEER_BIND_PORT_FAILED << p->id << p->ip);
    }
}

void peers::peer_leave(int id, char* ip) {

}

Peer* peers::get_peer(int id) {
    for (int n = 0; n < peer_list.size(); ++n) {
        if (peer_list[n]->id == id) {
            return peer_list[n];
        }
    }
    return NULL;
}

Peer* peers::get_peer(Socket& sock) {
    for (int n = 0; n < peer_list.size(); ++n) {
        if (peer_list[n]->udp_sock == &sock) {
            return peer_list[n];
        }
    }
    return NULL;
}