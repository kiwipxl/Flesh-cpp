#include "GameMessages.h"
#include "../StateManager.h"
#include "sockets/SocketManager.h"
#include "Peers.h"

using err::fresult;

std::thread msg::game::msgs_thread;
SocketPoll msg::game::server_poll;
bool msg::game::accepting_peers = true;

#define VALID_PARAMS(a, b) a == b && msg::param_list_size >= b->num_params

void recv_msgs() {
    using namespace msg::game;

    server_poll.add_sock(sock::tcp_serv_sock);

	char buffer[1024];
	int msg_len;
	while (true) {
        if (accepting_peers) {
            //for (int p = 0; p < peers.size(); ++p) {

            //}
        }

		int total = 0;
		if ((total = server_poll.poll()) > 0) {
            int size = server_poll.get_size();
            for (int i = 0; i < size; ++i) {
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
						CMID mid = msg::extract_mid(buffer, msg_len);
						if (mid->id > 0 && mid->id < msg::MID_list.size()) {
							msg::extract_params(mid, buffer, msg_len);

							if (VALID_PARAMS(mid, _MID->RELAY_TEST)) {
								int* a = (int*)msg::param_list[0]->data;
                                char** b = &msg::param_list[1]->data;
								u_short* c = (u_short*)msg::param_list[2]->data;
								u_short* d = (u_short*)msg::param_list[3]->data;

                                msg::print_extracted_params();

								msg::send(*sock, msg::ByteStream() << _MID->RELAY_TEST << *a << msg::param_list[1] << *c << *d);

								std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							}else if (VALID_PARAMS(mid, _MID->RECV_ID)) {
								msg::print_extracted_params();
							}else if (VALID_PARAMS(mid, _MID->RECV_SERVER_BINDED_UDP_PORT)) {
                                msg::print_extracted_params();
                                
                                if (sock::setup_udp_sock(*(u_short*)msg::param_list[0]->data)) {
                                    msg::send(sock::tcp_serv_sock, msg::ByteStream() << _MID->SEND_CLIENT_BINDED_UDP_PORT << sock::udp_serv_sock.get_binded_port());
                                    server_poll.add_sock(sock::udp_serv_sock);
                                    sock::send_udp_ping_pong(sock::udp_serv_sock);
                                }else {
                                    sock::connection_finished = true;
                                    sock::connection_error = -1;
                                }
                            }else if (VALID_PARAMS(mid, _MID->UDP_INIT_PING_PONG)) {
                                sock::udp_ping_pong = false;
                                sock::connection_finished = true;
                                sock::connection_error = NO_ERROR;

                                msg::send(sock::tcp_serv_sock, msg::ByteStream() << _MID->SEND_UDP_SERVER_COMMUNICATION_SUCCESS);
                            }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_BIND_REQUEST)) {

                            }else if (VALID_PARAMS(mid, _MID->UDP_PING_PONG)) {
                                CCLOG("caught udp ping pong!");
                            }else if (VALID_PARAMS(mid, _MID->RECV_PEER_JOIN)) {
                                peers::peer_join(*(int*)msg::param_list[0]->data, msg::param_list[1]->data, *(u_short*)msg::param_list[2]->data);
                            }else if (VALID_PARAMS(mid, _MID->RECV_PEER_LEAVE)) {
                                peers::peer_leave(*(int*)msg::param_list[0]->data, msg::param_list[1]->data, *(u_short*)msg::param_list[2]->data);
							}
							msg::clear_param_list();
						}
					}
				}
			}
		}else if (total == -1) {
			CCLOG("polling error occurred: %d", err::get_last_error());
		}
	}
}

void msg::game::close_all_threads() {
    msgs_thread.detach();
}

void msg::game::start_recv_thread() {
    msgs_thread = std::thread(recv_msgs);
}