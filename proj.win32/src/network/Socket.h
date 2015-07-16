#ifndef __SOCKET_H__
#define __SOCKET_H__

/*
simple cross-platform berkeley socket class used to encapsulate simpler functions
*/

#include "debug/PlatformConfig.h"

#if defined(PLATFORM_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(PLATFORM_ANDROID)
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

enum SocketProtocol {
	PROTO_TCP, 
	PROTO_UDP
};

class Socket {

	public:
		static void init_sockets();

		Socket(SocketProtocol c_protocol = PROTO_TCP, char* c_ip = "n/a", char* c_port = "n/a");
		char* ip;
		char* port;
		SocketProtocol protocol;

		bool try_connect();
		bool send_buffer(char* buffer, int buffer_len);
		bool try_listen();

		uintptr_t get_sock() { return sock; }

	private:
		int err;
		uintptr_t sock;
		addrinfo* result;
		addrinfo hints;
};

#endif
