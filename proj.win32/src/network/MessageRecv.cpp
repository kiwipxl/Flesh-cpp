#include "MessageRecv.h"
#include <thread>
#include "../debug/Errors.h"

Socket messagerecv::tcp_sock;
Socket messagerecv::udp_sock;
std::thread messagerecv::recv_thread;

#define VALID_PARAMS(a, b) a == b && message::param_list_size >= b->num_params

void messagerecv::tcp_recv() {
	fd_set read_list;
	fd_set write_list;

	struct pollfd fds[1];
	fds[0].fd = tcp_sock.get_sock();
	fds[0].events = POLLRDNORM | POLLRDBAND;

	char buffer[1024];
	int msg_len;
	while (true) {
		int total = 0;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		if ((total = tcp_sock.s_poll(fds, 1, 1000)) > 0) {
			int i = 0;
			if (fds[i].revents & POLLERR) {
				CCLOG("poll error occurred");
			}else if (fds[i].revents & POLLHUP) {
				CCLOG("poll hang up error occurred");
			}else if (fds[i].revents & POLLNVAL) {
				CCLOG("poll invalid request occurred");
			}else if (fds[i].revents & POLLRDNORM || fds[i].revents & POLLRDBAND || fds[i].revents & POLLIN || fds[i].revents & POLLPRI) {
				if ((msg_len = tcp_sock.s_recv(buffer, 1024)) > 0) {
					CMID mid = message::extract_mid(buffer, msg_len);
					if (mid->id > 0 && mid->id < message::MID_list.size()) {
						message::extract_params(mid, buffer, msg_len);

						if (VALID_PARAMS(mid, message::MID_RELAY_TEST)) {
							bool* a = (bool*)message::param_list[0]->data;
							bool* b = (bool*)message::param_list[1]->data;
							int* c = (int*)message::param_list[2]->data;
							float* d = (float*)message::param_list[3]->data;
							char** e = &message::param_list[4]->data;

							message::print_extracted_params();

							int r = ((std::rand() / (float)RAND_MAX) * 100.0f);
							message::send(&tcp_sock, message::ByteStream() << message::MID_RELAY_TEST << *a << *b << r << *d << *e);
						}else if (VALID_PARAMS(mid, message::MID_CLIENT_ID)) {
							message::print_extracted_params();
						}
						message::clear_param_list();
					}
				}
			}
		}
	}
}

void start_failed() {
	CCLOG("socket start failed!");
}

void messagerecv::start() {
	message::init();
	Socket::init_sockets();

	int fresult;
	tcp_sock = Socket(PROTO_TCP, "192.168.0.5", "4222");
	if ((fresult = tcp_sock.s_create()) != NO_ERROR) { CCLOG("(tcp_sock): error %d occurred while creating tcp socket", fresult); start_failed(); return; }
	if ((fresult = tcp_sock.s_connect()) != NO_ERROR) { CCLOG("(tcp_sock): error %d occurred while trying to connect to (ip: %s, port: %s)", fresult, tcp_sock.get_ip(), tcp_sock.get_port()); start_failed(); return; }

	udp_sock = Socket(PROTO_UDP, "192.168.0.2", "4222");
	if ((fresult = udp_sock.s_create()) != NO_ERROR) { CCLOG("(udp_sock): error %d occurred while creating tcp socket", fresult); start_failed(); return; }
	if ((fresult = udp_sock.s_bind()) != NO_ERROR) { CCLOG("(udp_sock): error %d occurred while trying to bind to (ip: %s, port: %s)", fresult, udp_sock.get_ip(), udp_sock.get_port()); start_failed(); return; }

	CCLOG("socket start was successful");

	recv_thread = std::thread(messagerecv::tcp_recv);

	message::send(&tcp_sock, message::ByteStream() << message::MID_RELAY_TEST << false << true << 458 << 89.42f << "debug12");
}