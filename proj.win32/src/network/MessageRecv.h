#ifndef _MESSAGE_RECV_H_
#define _MESSAGE_RECV_H_

#include "Socket.h"
#include "Message.h"

namespace messagerecv {

	extern Socket tcp_serv_sock;
	extern Socket udp_serv_recv_sock;
	extern Socket udp_serv_send_sock;
	extern std::thread recv_thread;

	extern void start();
	extern void tcp_recv();
}

#endif