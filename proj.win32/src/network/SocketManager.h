#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

/*
handles the tcp and udp socket connection between the server and udp sockets between peers
*/

#include "Socket.h"
#include "Message.h"
#include "MessageRecv.h"
#include "../debug/Errors.h"

namespace sock {

    extern Socket tcp_serv_sock;
    extern Socket udp_serv_sock;
    extern std::thread msg_recv_thread;
    extern std::thread tcp_connect_thread;

    extern int connection_error;

    extern void init();
    extern void setup_tcp_sock();
    extern void socket_setup_failed(int err);
    extern void setup_udp_sock(u_short udp_recv_port, u_short udp_serv_port);
};

#endif