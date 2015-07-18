#include "MessageRecv.h"
#include <thread>

Socket messagerecv::tcp_sock;
Socket messagerecv::udp_sock;
std::thread messagerecv::recv_thread;

void messagerecv::tcp_recv() {
	fd_set read_list;
	fd_set write_list;
	tcp_sock.s_setup_select(&read_list, &write_list, 0, 1);

	char buffer[1024];
	int msg_len;
	while (true) {
		int total = 0;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if ((total = tcp_sock.s_select()) > 0) {
			if ((msg_len = recv(read_list.fd_array[0], buffer, 1024, 0)) > 0) {
				CMID mid = message::extract_mid(buffer, msg_len);
				if (mid->id > 0 && mid->id < message::MID_list.size()) {
					message::extract_params(mid, buffer, msg_len);
					if (mid == message::MID_RELAY_TEST && message::param_list_size >= message::MID_RELAY_TEST->num_params) {
						bool* a = (bool*)message::param_list[0];
						bool* b = (bool*)message::param_list[1];
						int* c = (int*)message::param_list[2];
						float* d = (float*)message::param_list[3];
						char* e = (char*)message::param_list[4];

						CCLOG("b: %d, b: %d, i: %d, f: %f, c: %s", *a, *b, *c, *d, e);

						message::send(&tcp_sock, message::ByteStream() << message::MID_RELAY_TEST << *a << *b << *c << *d << e);
					}
					message::clear_param_list();
				}
			}
		}
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

	recv_thread = std::thread(messagerecv::tcp_recv);

	message::send(&tcp_sock, message::ByteStream() << message::MID_RELAY_TEST << false << true << 458 << 89.42f << "debug12");
}