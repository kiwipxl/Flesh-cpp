#include "GameMessages.h"
#include "../StateManager.h"
#include "SocketManager.h"

using err::fresult;

std::thread game_msgs::msgs_thread;
SocketPoll game_msgs::server_poll;

#define VALID_PARAMS(a, b) a == b && message::param_list_size >= b->num_params

void recv_msgs() {
    using namespace game_msgs;

    server_poll.add_sock(sock::tcp_serv_sock);

	char buffer[1024];
	int msg_len;
	while (true) {
		int total = 0;
		if ((total = server_poll.poll()) > 0) {
            for (int i = 0; i < server_poll.get_size(); ++i) {
                int revents = server_poll.get_fd_at(i)->revents;
				if (revents & POLLERR) {
					CCLOG("poll error occurred");
				}else if (revents & POLLHUP) {
					CCLOG("poll hang up error occurred");
				}else if (revents & POLLNVAL) {
					CCLOG("poll invalid request occurred");
				}else if (revents & POLLRDNORM || revents & POLLRDBAND || revents & POLLIN || revents & POLLPRI) {
                    Socket* sock = server_poll.get_sock_at(i);
                    if (!sock) continue;
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
                                
                                if (sock::setup_udp_sock(*(u_short*)message::param_list[0]->data, *(u_short*)message::param_list[1]->data)) {
                                    server_poll.add_sock(sock::udp_serv_sock);
                                    sock::connection_finished = true;
                                    sock::connection_error = NO_ERROR;
                                }else {
                                    sock::connection_finished = true;
                                    sock::connection_error = -1;
                                }
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

void game_msgs::start_recv_thread() {
    msgs_thread = std::thread(recv_msgs);
}