#ifndef __SOCKET_H__
#define __SOCKET_H__

/*
simple cross-platform berkeley socket class used to encapsulate simpler functions
*/

#include <WinSock2.h>
#include <WS2tcpip.h>

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

		SOCKET get_sock() { return sock; }

	private:
		int err;
		SOCKET sock;
		struct addrinfo* result;
		struct addrinfo hints;
};

#endif
