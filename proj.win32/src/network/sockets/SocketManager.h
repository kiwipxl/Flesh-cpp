#ifndef _SOCKET_MANAGER_H_
#define _SOCKET_MANAGER_H_

/*
handles the tcp and udp socket connection between the server and udp sockets between peers
*/

#include <thread>

#include "network/Defines.h"
#include "debug/Errors.h"

BEGIN_NETWORK_NS
BEGIN_SOCK_NS

extern Socket tcp_serv_sock;
extern Socket udp_serv_sock;
extern std::thread tcp_connect_thread;

extern bool udp_ping_pong;
extern float udp_ping_pong_time;
extern int udp_ping_pong_tries;
extern Socket* udp_ping_pong_sock;
#define MAX_UDP_PING_PONG_TRIES 3
#define UDP_PING_PONG_TIMEOUT 2.5f

extern char* SERVER_IP;
extern char* LOCAL_SERVER_IP;
extern char* serv_ip;
extern u_short serv_port;

extern bool connection_finished;
extern int connection_err;
extern std::string connection_err_msg;

extern void init();
extern void update();
extern void setup_tcp_sock();
extern void socket_setup_failed(int err);
extern bool setup_udp_sock(u_short udp_serv_port);
extern void send_udp_ping_pong(Socket& sock);
extern void begin_relay_test(Socket& sock);
extern void close_all_threads();
extern void cleanup_all();

END_SOCK_NS
END_NETWORK_NS

#endif