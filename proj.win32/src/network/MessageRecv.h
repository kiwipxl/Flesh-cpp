#ifndef _MESSAGE_RECV_H_
#define _MESSAGE_RECV_H_

#include "Socket.h"
#include "Message.h"

namespace messagerecv {

	extern Socket tcp_sock;
	extern Socket udp_sock;

	extern void start();
}

#endif