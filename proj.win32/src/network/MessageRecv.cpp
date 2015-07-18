#include "MessageRecv.h"
#include <thread>

Socket messagerecv::tcp_sock;
Socket messagerecv::udp_sock;
std::thread messagerecv::recv_thread;

void messagerecv::tcp_recv() {
	fd_set read_list;
	tcp_sock.s_setup_select(&read_list, NULL, 1, 0);

	char buffer[1024];
	int msg_len;
	while (true) {
		CCLOG("receiving: %d", tcp_sock.s_select());
		/*if ((msg_len = tcp_sock.s_recv(buffer, 1024)) > 0) {
			CCLOG("buffer: %s, len: %d", buffer, msg_len);
		}*/
	}
}

void messagerecv::start() {
	Socket::init_sockets();

	tcp_sock = Socket(PROTO_TCP, "192.168.0.5", "4222");
	tcp_sock.s_create();
	tcp_sock.s_connect();
	
	udp_sock = Socket(PROTO_UDP, "192.168.0.2", "4222");
	udp_sock.s_create();
	udp_sock.s_bind();

	message::ByteStream() << message::MID_CLIENT_USER_PASS << false << true << 458 << 89.42f << "testtttttttt232!#$@";
	CMID mid = message::extract_mid(message::byte_buffer, message::byte_offset);
	message::extract_params(mid, message::byte_buffer, message::byte_offset);

	bool* a = (bool*)message::param_list[0];
	bool* b = (bool*)message::param_list[1];
	int* c = (int*)message::param_list[2];
	float* d = (float*)message::param_list[3];
	char* e = (char*)message::param_list[4];

	CCLOG("bool: %d, bool2: %d, int: %d, float: %f, str: %s", *a, *b, *c, *d, e);

	message::clear_param_list();

	recv_thread = std::thread(messagerecv::tcp_recv);

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		message::send(&tcp_sock, message::ByteStream() << message::MID_CLIENT_USER_PASS << false << true << 458 << 89.42f << "debug12");
	}
}