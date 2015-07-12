#include "Socket.h"
#include "cocos2d.h"

Socket::Socket(SocketProtocol c_protocol, char* c_ip, char* c_port) {
	WSAData wsa_data;
	SOCKET sock = INVALID_SOCKET;
	struct addrinfo* result, hints;
	int err;
	char* ip = "127.0.0.1";
	char* port = "4222";

	if ((err = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
		CCLOG("WSA Startup failed! Err: %d", err);
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	switch (protocol) {
		case PROTO_TCP:
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			break;
		case PROTO_UDP:
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_protocol = IPPROTO_UDP;
			break;
	}
}

bool Socket::try_connect() {
	if ((err = getaddrinfo(ip, port, &hints, &result)) != 0) {
		CCLOG("getaddrinfo failed (ip: %s, port: %s). Err %d", ip, port, err);
		return false;
	}

	while (result != NULL) {
		if ((sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) == INVALID_SOCKET) {
			CCLOG("connection failed, invalid socket (ip: %s, port: %s). Err %d", ip, port, err);
			return false;
		}
		if ((err = connect(sock, result->ai_addr, (int)result->ai_addrlen)) == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			result = result->ai_next;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		CCLOG("could not connect (ip: %s, port: %s). Err %d", ip, port, err);
		return false;
	}
	return true;
}

bool Socket::send_buffer() {
	char* buffer = "ayy lmao";
	send(sock, buffer, strlen(buffer), 0);
	CCLOG("sent");
}