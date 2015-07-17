#include "MessageRecv.h"

Socket messagerecv::tcp_sock;
Socket messagerecv::udp_sock;

void messagerecv::start() {
	Socket::init_sockets();

	tcp_sock = Socket(PROTO_TCP, "192.168.0.5", "4222");
	tcp_sock.s_create();
	tcp_sock.s_connect();
	
	udp_sock = Socket(PROTO_UDP, "192.168.0.2", "4222");
	udp_sock.s_create();
	udp_sock.s_bind();

	message::ByteStream() << message::MID_CLIENT_USER_PASS << false << true << 458 << 89.42f;
	CMID mid = message::extract_mid(message::byte_buffer, message::byte_offset);
	message::extract_params(mid, message::byte_buffer, message::byte_offset);

	bool* a = (bool*)message::param_list[0];
	bool* b = (bool*)message::param_list[1];
	int* c = (int*)message::param_list[2];
	float* d = (float*)message::param_list[3];

	message::clear_param_list();

	message::send(&tcp_sock, message::ByteStream() << message::MID_CLIENT_USER_PASS << false << true);

	char buffer[1024];
	int msg_len;
		while (true) {
		if ((msg_len = tcp_sock.s_recv(buffer, sizeof(buffer))) > 0) {
			CCLOG("buffer: %s", buffer);
		}
	}
}