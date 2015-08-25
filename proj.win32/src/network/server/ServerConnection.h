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

extern char* SERVER_IP;
extern char* LOCAL_SERVER_IP;
extern char* serv_ip;
extern u_short serv_port;

extern bool connection_finished;
extern int connection_err;
extern std::string connection_err_msg;

extern void init();
extern void setup_tcp_sock();
extern void socket_setup_failed(int err);
extern bool setup_udp_sock(u_short udp_serv_port);
extern void close_all_threads();
extern void cleanup_all();

END_SERVER_NS
END_NETWORK_NS

#endif