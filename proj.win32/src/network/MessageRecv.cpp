#include "MessageRecv.h"
#include <thread>
#include <cstdint>
#include "../debug/Errors.h"
#include "../State_Manager.h"

Socket messagerecv::tcp_serv_sock;
Socket messagerecv::udp_serv_sock;
std::thread messagerecv::msg_recv_thread;
std::thread messagerecv::tcp_connect_thread;

bool messagerecv::done_connecting = false;
int messagerecv::connect_result = -1;

int fresult;

#define VALID_PARAMS(a, b) a == b && message::param_list_size >= b->num_params

void messagerecv::recv_msgs() {
	fd_set read_list;
	fd_set write_list;
	
	std::vector<pollfd> fds;
	fds.resize(2);
	fds[0].fd = tcp_serv_sock.get_sock();
	fds[0].events = POLLRDNORM | POLLRDBAND;
	fds[1].fd = udp_serv_sock.get_sock();
	fds[1].events = POLLRDNORM | POLLRDBAND;

	std::vector<Socket*> socket_fds;
	socket_fds.resize(fds.size());
	socket_fds[0] = &tcp_serv_sock;
	socket_fds[1] = &udp_serv_sock;

	char buffer[1024];
	int msg_len;
	while (true) {
		int total = 0;
		if ((total = Socket::poll_fds(&fds[0], fds.size(), 1000)) > 0) {
			for (int i = 0; i < fds.size(); ++i) {
				if (fds[i].revents & POLLERR) {
					CCLOG("poll error occurred");
				}else if (fds[i].revents & POLLHUP) {
					CCLOG("poll hang up error occurred");
				}else if (fds[i].revents & POLLNVAL) {
					CCLOG("poll invalid request occurred");
				}else if (fds[i].revents & POLLRDNORM || fds[i].revents & POLLRDBAND || fds[i].revents & POLLIN || fds[i].revents & POLLPRI) {
					Socket* sock = socket_fds[i];
					if ((msg_len = sock->s_recv(buffer, 1024)) > 0) {
						CMID mid = message::extract_mid(buffer, msg_len);
						if (mid->id > 0 && mid->id < message::MID_list.size()) {
							message::extract_params(mid, buffer, msg_len);

							if (VALID_PARAMS(mid, message::MID_RELAY_TEST)) {
								int* a = (int*)message::param_list[0]->data;
                                char** b = &message::param_list[1]->data;
								u_short* c = (u_short*)message::param_list[2]->data;
								u_short* d = (u_short*)message::param_list[3]->data;

								message::print_extracted_params();

								message::send(sock, message::ByteStream() << message::MID_RELAY_TEST << *a << message::param_list[1] << *c << *d);

								std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							}else if (VALID_PARAMS(mid, message::MID_CLIENT_ID)) {
								message::print_extracted_params();
							}else if (VALID_PARAMS(mid, message::MID_GET_TCP_AND_UDP_CLIENT_PORTS)) {
								message::print_extracted_params();

								udp_serv_sock.s_change_addr("0.0.0.0", *(unsigned short*)message::param_list[0]->data);
								if ((fresult = udp_serv_sock.s_bind()) != NO_ERROR) {
                                    CCLOG("(udp_serv_sock): error %d occurred while trying to bind to (ip: %s, port: %d)", fresult, udp_serv_sock.get_ip(), udp_serv_sock.get_port());
                                    socket_setup_failed(fresult);
                                }else {
                                    udp_serv_sock.s_change_addr("192.168.0.2", *(unsigned short*)message::param_list[1]->data);
                                    message::send(&udp_serv_sock, message::ByteStream() << message::MID_BEGIN_RELAY_TEST);
                                    message::send(&tcp_serv_sock, message::ByteStream() << message::MID_BEGIN_RELAY_TEST);

                                    done_connecting = true;
                                    connect_result = NO_ERROR;
                                }
							}
							message::clear_param_list();
						}
					}
				}
			}
		}else if (total == -1) {
			CCLOG("polling error occurred: %d", get_last_error());
		}
	}
}

void messagerecv::tcp_connect() {
    CCLOG("attempt connect on thread...");
	tcp_serv_sock = Socket(PROTO_TCP, "192.168.0.2", 4222);
	if ((fresult = tcp_serv_sock.s_create()) != NO_ERROR) {
		CCLOG("(tcp_serv_sock): error %d occurred while creating tcp socket", fresult);
		socket_setup_failed(fresult); return;
	}
	if ((fresult = tcp_serv_sock.s_connect()) != NO_ERROR) {
		CCLOG("(tcp_serv_sock): error %d occurred while trying to connect to (ip: %s, port: %d)", fresult, tcp_serv_sock.get_ip(), tcp_serv_sock.get_port());
		socket_setup_failed(fresult); return;
	}

	CCLOG("(tcp_serv_sock): connection successful");

	udp_serv_sock = Socket(PROTO_UDP, "0.0.0.0", 0);
	if ((fresult = udp_serv_sock.s_create()) != NO_ERROR) {
		CCLOG("(udp_serv_sock): error %d occurred while creating tcp socket", fresult);
		socket_setup_failed(fresult); return;
	}

    CCLOG("(udp_serv_sock): creation/binding successful");

    msg_recv_thread = std::thread(messagerecv::recv_msgs);
}

void messagerecv::socket_setup_failed(int err) {
	done_connecting = true;
	connect_result = err;
}

void messagerecv::connect_to_server() {
	message::init();
    Socket::init_sockets();

    CCLOG("attempt connect...");
	tcp_connect_thread = std::thread(messagerecv::tcp_connect);
}