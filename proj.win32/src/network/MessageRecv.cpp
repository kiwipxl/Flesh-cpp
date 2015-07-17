#include "MessageRecv.h"

Socket tcp_sock;
Socket udp_sock;

void message_recv_start() {
	Socket::init_sockets();

	tcp_sock = Socket(PROTO_TCP, "192.168.0.2", "4222");
	tcp_sock.s_create();
	tcp_sock.s_connect();
	
	udp_sock = Socket(PROTO_UDP, "192.168.0.2", "4222");
	udp_sock.s_create();
	udp_sock.s_bind();

	char buffer[1024];
	int msg_len;
	while (true) {
		if ((msg_len = tcp_sock.s_recv(buffer, sizeof(buffer))) > 0) {
			CCLOG("buffer: %s", buffer);
		}
	}

	Msg::send(&tcp_sock, Msg::ByteStream() << MID_CLIENT_USER_PASS << false << true);
}