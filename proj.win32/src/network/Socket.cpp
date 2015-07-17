#include "Socket.h"
#include "Message.h"
#include "../debug/Errors.h"

void Socket::init_sockets() {
	#if defined(PLATFORM_WIN32)
	WSAData wsa_data;
	int err;
	if ((err = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
		CCLOG("WSA Startup failed! Sockets could not be initialised. Err: %d", err);
	}
	#endif
}

Socket::Socket(SocketProtocol c_protocol, char* c_ip, char* c_port) {
	protocol = c_protocol;
	ip = c_ip;
	port = c_port;

	memset(&sock_info, 0, sizeof(sock_info));
	sock_info.ai_family = AF_UNSPEC;
	switch (protocol) {
		case PROTO_TCP:
			sock_info.ai_socktype = SOCK_STREAM;
			sock_info.ai_protocol = IPPROTO_TCP;
			break;
		case PROTO_UDP:
			sock_info.ai_socktype = SOCK_DGRAM;
			sock_info.ai_protocol = IPPROTO_UDP;
			break;
	}
}

int Socket::s_create() {
	//setup serv_addr structure
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(port));

	//convert ip and copy into ipv4 structure in_addr
	if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) return get_last_error();
	//create socket
	if ((sock = socket(AF_INET, sock_info.ai_socktype, sock_info.ai_protocol)) < 0) return get_last_error();

	return NO_ERROR;
}

int Socket::s_bind() {
	if ((sock = bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) return get_last_error();
	return NO_ERROR;
}

int Socket::s_connect() {
	if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) return get_last_error();

	return NO_ERROR;
}

//bool Socket::s_listen() {
	/*sock = socket(sock_info.ai_family, sock_info.ai_socktype, sock_info.ai_protocol);

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
	}*/
	//return true;
//}

int Socket::s_send(char* buffer, int buffer_len) {
	if (protocol == PROTO_TCP) {
		if ((result = send(sock, buffer, buffer_len, 0)) < 0) return get_last_error();
	}else if (protocol == PROTO_UDP) {
		if ((result = sendto(sock, buffer, buffer_len, 0, (sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) return get_last_error();
	}
	return result;
}

int Socket::s_recv(char* buffer, int buffer_len) {
	if (protocol == PROTO_TCP) {
		if ((result = recv(sock, buffer, buffer_len, 0)) < 0) return get_last_error();
	}else if (protocol == PROTO_UDP) {
		if ((result = recvfrom(sock, buffer, buffer_len, 0, (sockaddr*)&serv_addr, (int*)sizeof(serv_addr))) < 0) return get_last_error();
	}
	return result;
}