#ifndef _MESSAGE_RECV_H_
#define _MESSAGE_RECV_H_

#include "Socket.h"
#include "Message.h"

namespace messagerecv {

	extern Socket tcp_sock;
	extern Socket udp_sock;
	extern std::thread recv_thread;

	extern void start();
	extern void tcp_recv();
}

#endif