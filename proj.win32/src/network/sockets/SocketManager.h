#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

/*
handles the tcp and udp socket connection between the server and udp sockets between peers
*/

#include "Socket.h"
#include "../Message.h"
#include "../GameMessages.h"
#include "../../debug/Errors.h"

namespace sock {

    extern Socket tcp_serv_sock;
    extern Socket udp_serv_sock;
    extern std::thread tcp_connect_thread;

    extern bool udp_ping_pong;
    extern float udp_ping_pong_time;
    extern int udp_ping_pong_tries;
    #define MAX_UDP_PING_PONG_TRIES 3
    #define UDP_PING_PONG_TIMEOUT 2.5f

    extern bool connection_finished;
    extern int connection_error;

    extern void init();
    extern void update();
    extern void setup_tcp_sock();
    extern void socket_setup_failed(int err);
    extern bool setup_udp_sock(u_short udp_recv_port, u_short udp_serv_port);
    extern void send_udp_ping_pong();
    extern void begin_relay_test(Socket& sock);
    extern void close_all_threads();
};

#endif