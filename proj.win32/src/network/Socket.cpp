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

int Socket::s_select(fd_set* read_set, fd_set* write_set, bool use_timeout, int timeout_seconds, int timeout_ms) {
	if (read_set != NULL) {
		FD_ZERO(read_set);
		FD_SET(sock, read_set);
	}
	if (write_set != NULL) FD_ZERO(write_set);

	t.tv_sec = timeout_seconds;
	t.tv_usec = timeout_ms;

	return select(2, read_set, NULL, NULL, use_timeout ? &t : NULL);
}

int Socket::s_poll(pollfd* fd_array, int array_len, int timeout) {
	#if defined(PLATFORM_WIN32)
		return WSAPoll(fd_array, array_len, timeout);
	#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)
		return poll(fd_array, array_len, timeout);
	#endif
}

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