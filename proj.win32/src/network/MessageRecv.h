#ifndef _MESSAGE_RECV_H_
#define _MESSAGE_RECV_H_

#include "Socket.h"
#include "Message.h"

namespace messagerecv {

	extern Socket tcp_serv_sock;
	extern Socket udp_serv_sock;
	extern std::thread msg_recv_thread;
	extern std::thread tcp_connect_thread;

	extern bool done_connecting;
	extern int connect_result;

    extern void connect_to_server();
	extern void recv_msgs();
	extern void tcp_connect();
	extern void socket_setup_failed(int err);
}

#endif