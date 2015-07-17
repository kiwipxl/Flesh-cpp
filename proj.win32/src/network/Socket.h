#ifndef __SOCKET_H__
#define __SOCKET_H__

/*
simple cross-platform berkeley socket class used to encapsulate simpler functions
*/

#include <base/CCConsole.h>
#include "debug/PlatformConfig.h"

#if defined(PLATFORM_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
		SocketProtocol protocol;

		int s_create();
		int s_bind();
		int s_connect();
		int s_send(char* buffer, int buffer_len);
		int s_recv(char* buffer, int buffer_len);

		uintptr_t get_sock() { return sock; }
		char* get_ip() { return ip; }
		char* get_port() { return port; }

	private:
		int result;
		char* ip;
		char* port;
		uintptr_t sock;
		struct sockaddr_in serv_addr;
		struct addrinfo sock_info;
};

#endif
