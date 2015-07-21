#include "SocketPoll.h"
#include <thread>
#include <algorithm>
#include "../StateManager.h"
#include "SocketManager.h"

using err::fresult;

#define VALID_PARAMS(a, b) a == b && message::param_list_size >= b->num_params

int SocketPoll::poll() {
    return Socket::poll_fds(fds[0], fds.size(), 1000);
}

void SocketPoll::add_sock(Socket& sock) {
    pollfd fd;
    fds.push_back(&fd);
    fd.fd = sock.get_sock();
    fd.events = POLLRDNORM | POLLRDBAND;
}

void SocketPoll::remove_sock(Socket& sock) {
    sockets.erase(std::remove(sockets.begin(), sockets.end(), sock), sockets.end());
}

void SocketPoll::remove_sock(int at) {
    for (int n = 0; n < sockets.size(); ++n) {
        if (n == at) {
            sockets.erase(sockets.begin() + n, sockets.end() + n + 1);
        }
    }
}

void SocketPoll::recv_msgs() {
	fd_set read_list;
	fd_set write_list;
	
	std::vector<pollfd> fds;
	fds.resize(2);
	fds[0].fd = sock::tcp_serv_sock.get_sock();
	fds[0].events = POLLRDNORM | POLLRDBAND;
	fds[1].fd = sock::udp_serv_sock.get_sock();
	fds[1].events = POLLRDNORM | POLLRDBAND;

	std::vector<Socket*> socket_fds;
	socket_fds.resize(fds.size());
	socket_fds[0] = &sock::tcp_serv_sock;
	socket_fds[1] = &sock::udp_serv_sock;

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

                                sock::setup_udp_sock(*(u_short*)message::param_list[0]->data, *(u_short*)message::param_list[1]->data);
							}
							message::clear_param_list();
						}
					}
				}
			}
		}else if (total == -1) {
			CCLOG("polling error occurred: %d", err::get_last_error());
		}
	}
}