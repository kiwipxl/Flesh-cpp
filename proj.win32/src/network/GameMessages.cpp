#include "GameMessages.h"

#include "../StateManager.h"
#include "sockets/SocketManager.h"
#include "Peers.h"
#include "../entities/Unit.h"
#include "../debug/Logger.h"

using debug::fresult;

std::thread msg::game::msgs_thread;
SocketPoll msg::game::server_poll;
bool msg::game::accepting_peers = true;

#define VALID_PARAMS(a, b) a == b && msg::last_param_list_size >= b->num_params

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
                    log_warning << "poll error occurred";
				}else if (revents & POLLHUP) {
                    log_warning << "poll hang up error occurred";
				}else if (revents & POLLNVAL) {
                    log_warning << "poll invalid request occurred";
				}else if (revents & POLLRDNORM || revents & POLLRDBAND || revents & POLLIN || revents & POLLPRI) {
                    Socket* sock = server_poll.get_sock_at(i);
                    if (!sock) continue;
                    peers::Peer* peer;

					if ((msg_len = sock->s_recv(buffer, 1024)) > 0) {
                        msg::extract_msg(buffer, msg_len);
                        CMID mid = msg::last_MID;
						if (mid != _MID->UNKNOWN) {
                            msg::print_extracted_params(false, true);

							if (VALID_PARAMS(mid, _MID->RELAY_TEST)) {
								int* a = (int*)msg::last_param_list[0]->data;
                                char** b = &msg::last_param_list[1]->data;
								u_short* c = (u_short*)msg::last_param_list[2]->data;
								u_short* d = (u_short*)msg::last_param_list[3]->data;

                                msg::print_extracted_params();

								msg::send(*sock, msg::MsgStream() << _MID->RELAY_TEST << *a << msg::last_param_list[1] << *c << *d);

								std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							}else if (VALID_PARAMS(mid, _MID->RECV_ID)) {
								msg::print_extracted_params();
							}else if (VALID_PARAMS(mid, _MID->RECV_SERVER_BINDED_UDP_PORT)) {
                                msg::print_extracted_params();
                                
                                if (sock::setup_udp_sock(*(u_short*)msg::last_param_list[0]->data)) {
                                    msg::send(sock::tcp_serv_sock, msg::MsgStream() << _MID->SEND_CLIENT_BINDED_UDP_PORT << sock::udp_serv_sock.get_binded_port());
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

                                msg::send(sock::tcp_serv_sock, msg::MsgStream() << _MID->SEND_UDP_SERVER_COMMUNICATION_SUCCESS);
                            }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_BIND_REQUEST)) {
                                peers::peer_join(*(int*)msg::last_param_list[0]->data, msg::last_param_list[1]->data);

                                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                            }else if (VALID_PARAMS(mid, _MID->UDP_PING_PONG)) {
                                log_info << "caught udp ping pong!";
                            }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_PORT)) {
                                peer = peers::get_peer(*(int*)msg::last_param_list[0]->data);
                                if (peer != NULL && !peer->connected) {
                                    peer->udp_send_port = *(u_short*)msg::last_param_list[1]->data;
                                    peer->udp_sock->s_change_send_addr("127.0.0.1", peer->udp_send_port);
                                    server_poll.add_sock(*peer->udp_sock);
                                    peer->connected = true;
                                    msg::send(*peer->udp_sock, msg::MsgStream() << _MID->PO_PING_CONNECT_TEST);

                                    log_info << "sending ping to peer (peer_id: " << peer->id << 
                                                ", peer_recv_port: " << peer->udp_recv_port << ", peer_send_port: " << peer->udp_send_port << ")";
                                }else {
                                    log_warning << "peer " << *(int*)msg::last_param_list[0]->data << " could not be found (recv_peer_port)";
                                }
                            }else if (VALID_PARAMS(mid, _MID->PO_PLAYER_MOVEMENT)) {
                                peer = peers::get_peer(*sock);
                                if (peer != NULL) {
                                    entity::test_peer_movement(peer, *(int*)msg::last_param_list[0]->data, *(int*)msg::last_param_list[1]->data, *(float*)msg::last_param_list[2]->data);
                                }
                            }else if (VALID_PARAMS(mid, _MID->PO_PING_CONNECT_TEST)) {
                                msg::send(*sock, msg::MsgStream() << _MID->PO_PONG_CONNECT_TEST);
                            }else if (VALID_PARAMS(mid, _MID->PO_PONG_CONNECT_TEST)) {
                                peer = peers::get_peer(*sock);
                                if (peer != NULL) {
                                    msg::send(sock::tcp_serv_sock, msg::MsgStream() << _MID->SEND_PEER_CONNECT_SUCCESS << peer->id << peer->ip);
                                    entity::test_peer_join(peer);
                                }else {
                                    log_warning << "peer could not be found (binded_port: " << sock->get_binded_port() << ")";
                                }
							}
							msg::clear_param_list();
						}
					}
				}
			}
		}else if (total == -1) {
            log_error << "polling error occurred: " << debug::get_last_error();
		}
	}
}

void msg::game::close_all_threads() {
    msgs_thread.detach();
}

void msg::game::start_recv_thread() {
    msgs_thread = std::thread(recv_msgs);
}