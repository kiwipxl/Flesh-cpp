#include "MessageRecv.h"
#include <thread>

Socket messagerecv::tcp_sock;
Socket messagerecv::udp_sock;
std::thread messagerecv::recv_thread;

void messagerecv::tcp_recv() {
	fd_set read_list;
	fd_set write_list;
	tcp_sock.s_setup_select(&read_list, &write_list);

	char buffer[1024];
	int msg_len;
	while (true) {
		int total = 0;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if ((total = tcp_sock.s_select()) > 0) {
			CCLOG("total: %d", total);
			if ((msg_len = recv(read_list.fd_array[0], buffer, 1024, 0)) > 0) {
				CCLOG("received!");
				CMID mid = message::extract_mid(buffer, msg_len);
				if (mid->id > 0 && mid->id < message::MID_list.size()) {
					message::extract_params(mid, buffer, msg_len);
					if (mid == message::MID_RELAY_TEST && message::param_list_size >= message::MID_RELAY_TEST->num_params) {
						bool* a = (bool*)message::param_list[0]->data;
						bool* b = (bool*)message::param_list[1]->data;
						int* c = (int*)message::param_list[2]->data;
						float* d = (float*)message::param_list[3]->data;
						char** e = &(char*)message::param_list[4]->data;

						message::print_extracted_params();

						int r = ((std::rand() / (float)RAND_MAX) * 100.0f);
						message::send(&tcp_sock, message::ByteStream() << message::MID_RELAY_TEST << *a << *b << r << *d << *e);

						char* buf = message::byte_buffer;
						int len = message::byte_offset;
						int i = 5;
					}
					message::clear_param_list();
				}
			}
		}
	}
}

void messagerecv::start() {
	message::init();
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