#ifndef _SERVER_CONNECTION_H_
#define _SERVER_CONNECTION_H_

/*
handles the tcp and udp socket connections between the server and the client
*/

#include <thread>

#include "network/Defines.h"
#include "debug/Errors.h"

BEGIN_NETWORK_NS
BEGIN_SERVER_NS

extern sock::Socket tcp_sock;
extern sock::Socket udp_sock;
extern std::thread tcp_connect_thread;

extern char* server_ip;
extern u_short server_tcp_port;
extern u_short server_udp_port;

extern void init();
extern void update();
extern void setup_tcp_sock(ServerConnectCallback _callback = nullptr);
extern void setup_udp_sock(u_short _server_udp_port, ServerConnectCallback _callback = nullptr);
extern void close_all_threads();
extern void cleanup_all();

extern bool is_connected();

END_SERVER_NS
END_NETWORK_NS

#endif