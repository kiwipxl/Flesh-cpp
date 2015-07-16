#include "Socket.h"
#include "Message.h"

void Socket::init_sockets() {
	WSAData wsa_data;
	int err;
	if ((err = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
		CCLOG("WSA Startup failed! Sockets could not be initialised. Err: %d", err);
	}
}

Socket::Socket(SocketProtocol c_protocol, char* c_ip, char* c_port) {
	sock = INVALID_SOCKET;
	protocol = c_protocol;
	ip = c_ip;
	port = c_port;

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

	Msg::send(this, Msg::ByteStream() << MID_CLIENT_USER_PASS << false << true);

	return true;
}

bool Socket::try_listen() {
	sock = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(4223);
	
	if (bind(sock, (PSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) {
		OutputDebugStringA("an error occurred while trying to bind\n");
		int err = WSAGetLastError();
		int a = 5;
	}

	FD_SET read_set;
	FD_SET write_set;

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);

	FD_SET(sock, &read_set);
	
	if (listen(sock, 5)) {
		OutputDebugStringA("an error occurred while trying to listen\n");
	}

	u_long non_block_flag = 1;
	ioctlsocket(sock, FIONBIO, &non_block_flag);

	char buffer[1024];
	while (true) {
		timeval t;
		t.tv_sec = 0;
		t.tv_usec = 1;
		int total = select(0, &read_set, &write_set, NULL, NULL);
		if (FD_ISSET(sock, &read_set)) {
			SOCKET s = accept(sock, NULL, NULL);
			recv(s, buffer, 1024, 0);
		}
		if (read_set.fd_count > 1) OutputDebugStringA("> 1!!!!!!\n");
		for (int n = 0; n < read_set.fd_count; ++n) {
			recv(read_set.fd_array[n], buffer, 1024, 0);
			OutputDebugStringA(buffer);
		}
	}
	return true;
}

bool Socket::send_buffer(char* buffer, int buffer_len) {
	return send(sock, buffer, buffer_len, 0);
}