#ifndef __SOCKET_H__
#define __SOCKET_H__

/*
simple cross-platform berkeley socket class used to encapsulate simpler functions
*/

#include <base/CCConsole.h>

#include "debug/PlatformConfig.h"
#include "../message/Callback.h"

#if defined(PLATFORM_WIN32)
#include <WinSock2.h>
#include <WS2tcpip.h>
#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#define PRINT_IF_ERROR 1

enum SocketProtocol {
    PROTO_TCP,
    PROTO_UDP
};

class Socket {

	public:
		static void init_sockets();
		static int poll_fds(pollfd* fd_array, int array_len, int timeout);

        Socket(SocketProtocol c_protocol = PROTO_TCP);

		int s_create();
		int s_bind(char* binding_ip, u_short binding_port);
		int s_connect(char* connect_ip, u_short connect_port);
		int s_send(char* buffer, int buffer_len);
		int s_recv(char* buffer, int buffer_len);
		int s_select(fd_set* read_set, fd_set* write_set, bool use_timeout = false, int timeout_seconds = 0, int timeout_ms = 0);
        int s_change_send_addr(char* sending_ip, u_short sending_port);

        std::vector<msg::MsgCallbackPtr> callbacks;
        void add_callback(msg::MsgCallbackPtr& msg_callback) {
            callbacks.push_back(msg_callback);
        }

		uintptr_t get_sock() { return sock; }
		char* get_binded_ip() { return binded_ip; }
        u_short get_binded_port() { return binded_port; }
        char* get_send_ip() { return send_ip; }
        u_short get_send_port() { return send_port; }
        addrinfo& get_sock_info() { return sock_info; }
        sockaddr_in& get_binded_addr_info() { return binded_addr_info; }
        sockaddr_in& get_send_addr_info() { return send_addr_info; }
        SocketProtocol get_protocol() { return protocol; }

	private:
		SocketProtocol protocol;
		int result;
        char* binded_ip;
        u_short binded_port;
        char* send_ip;
        u_short send_port;
		uintptr_t sock;
        struct sockaddr_in binded_addr_info;
        struct sockaddr_in send_addr_info;
        struct addrinfo sock_info;
		fd_set* r_set = NULL;
		fd_set* w_set = NULL;
        timeval t;

        int print_error(int err, char* func_err);
        int s_change_addr(sockaddr_in& addr_info, char* c_ip, u_short c_port);
        int s_update_addr_info(sockaddr_in& addr_info);
};

#endif
