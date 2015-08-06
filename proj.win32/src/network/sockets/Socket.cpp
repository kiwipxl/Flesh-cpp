#include "Socket.h"

#include "../message/Message.h"
#include "../message/Callback.h"
#include "../message/MID.h"
#include "../../debug/Errors.h"
#include "../../debug/Logger.h"
#include "SocketManager.h"

#define PRINT_OR_ERROR(str) PRINT_IF_ERROR ? print_error(debug::get_last_error(), str) : debug::get_last_error()

void Socket::init_sockets() {
	#if defined(PLATFORM_WIN32)
		WSAData wsa_data;
		int err;
		if ((err = WSAStartup(MAKEWORD(2, 2), &wsa_data)) != 0) {
            log_error << "WSA Startup failed! Sockets could not be initialised. Err: " << err;
		}
	#endif
}

int Socket::poll_fds(pollfd* fd_array, int array_len, int timeout) {
	#if defined(PLATFORM_WIN32)
		return WSAPoll(fd_array, array_len, timeout);
	#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)
		return poll(fd_array, array_len, timeout);
	#endif
}

Socket::Socket(SocketProtocol c_protocol) {
    protocol = c_protocol;

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

int Socket::print_error(int err, char* func_err) {
    log_error << err << ": " << func_err;
    return err;
}

int Socket::s_create() {
	//create socket
	if ((sock = socket(AF_INET, sock_info.ai_socktype, sock_info.ai_protocol)) < 0) return PRINT_OR_ERROR("socket creation error");

	return NO_ERROR;
}

int Socket::s_change_send_addr(char* sending_ip, u_short sending_port) {
    send_ip = sending_ip;
    send_port = sending_port;

    return s_change_addr(send_addr_info, send_ip, send_port);
}

int Socket::s_change_addr(sockaddr_in& addr_info, char* c_ip, u_short c_port) {
    //setup addr_info structure
    memset(&addr_info, 0, sizeof(addr_info));
    addr_info.sin_family = AF_INET;
    addr_info.sin_port = htons(c_port);

    //convert ip and copy into ipv4 structure in_addr
    if (inet_pton(AF_INET, c_ip, &addr_info.sin_addr) <= 0) return PRINT_OR_ERROR("presentation to network error");

    return NO_ERROR;
}

int Socket::s_update_addr_info(sockaddr_in& addr_info) {
    int name = sizeof(addr_info);
    if ((result = getsockname(sock, (sockaddr*)&addr_info, &name)) < 0) return PRINT_OR_ERROR("get sock name error");
    return NO_ERROR;
}

int Socket::s_bind(char* binding_ip, u_short binding_port) {
    binded_ip = binding_ip;
    binded_port = binding_port;
    s_change_addr(binded_addr_info, binded_ip, binded_port);

    if ((result = bind(sock, (sockaddr*)&binded_addr_info, sizeof(binded_addr_info))) < 0) return PRINT_OR_ERROR("bind error");

    s_update_addr_info(binded_addr_info);
    binded_port = ntohs(binded_addr_info.sin_port);

	return NO_ERROR;
}

int Socket::s_connect(char* connect_ip, u_short connect_port) {
    binded_ip = connect_ip;
    binded_port = connect_port;
    s_change_addr(binded_addr_info, binded_ip, binded_port);

    if (connect(sock, (sockaddr*)&binded_addr_info, sizeof(binded_addr_info)) < 0) return PRINT_OR_ERROR("connect error");

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

int Socket::s_send(char* buffer, int buffer_len) {
	if (protocol == PROTO_TCP) {
		if ((result = send(sock, buffer, buffer_len, 0)) < 0) return PRINT_OR_ERROR("send error");
	}else if (protocol == PROTO_UDP) {
		if ((result = sendto(sock, buffer, buffer_len, 0, (sockaddr*)&send_addr_info, sizeof(send_addr_info))) < 0) return PRINT_OR_ERROR("sendto error");
	}
	return result;
}

int Socket::s_recv(char* buffer, int buffer_len) {
	if (protocol == PROTO_TCP) {
		if ((result = recv(sock, buffer, buffer_len, 0)) < 0) return PRINT_OR_ERROR("recv error");
	}else if (protocol == PROTO_UDP) {
		int addr_info_size = sizeof(send_addr_info);
		if ((result = recvfrom(sock, buffer, buffer_len, 0, (sockaddr*)&send_addr_info, &addr_info_size)) < 0) return PRINT_OR_ERROR("recvfrom error");
	}
	return result;
}

void Socket::add_callback(msg::CallbackPtr& msg_callback) {
    callbacks.push_back(msg_callback);
}

void Socket::add_message_handler(msg::MID_enum mid, msg::CallbackFunc func) {
    callbacks.push_back(msg::make_MID_callback(msg::get_MID(mid), func));
}