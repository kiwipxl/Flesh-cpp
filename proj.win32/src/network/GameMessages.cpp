#include "GameMessages.h"

#include "../StateManager.h"
#include "sockets/SocketManager.h"
#include "Peers.h"
#include "../entities/Unit.h"
#include "../debug/Logger.h"
#include "message/Message.h"

using debug::fresult;

std::thread msg::game::msgs_thread;
SocketPoll msg::game::server_poll;

#define VALID_PARAMS(a, b) a == b && msg::last_param_list_size >= b->num_params

void recv_msgs() {
    using namespace msg::game;

    server_poll.add_sock(sock::tcp_serv_sock);

	char buffer[1024];
	int msg_len;
	while (true) {
        time_t t;
        for (int i = 0; i < server_poll.get_size(); ++i) {
            Socket* sock = server_poll.get_sock_at(i);
            if (!sock) continue;
            for (int n = 0; n < server_poll.get_sock_at(i)->callbacks.size(); ++n) {
                msg::CallbackPtr& cb = server_poll.get_sock_at(i)->callbacks[n];
                if (cb->timeout_len != msg::TIMEOUT_NONE && (time(&t) - cb->creation_time) >= cb->timeout_len) {
                    log_info << "callback timeout for " << cb->mid->name;
                    msg::Message m;
                    m.mid = msg::get_MID(msg::MID_UNKNOWN);
                    m.callback_result = msg::CALLBACK_RESULT_TIMEOUT;
                    cb->func(&m);
                    if (cb->remove_after_call) {
                        sock->callbacks.erase(sock->callbacks.begin() + n);
                        --n;
                    }else {
                        cb->reset_timeout();
                    }
                }
            }
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
                        msg::MessagePtr message = msg::extract_message(buffer, msg_len);
						if (message->mid->id != msg::MID_UNKNOWN) {
                            message->callback_result = msg::CALLBACK_RESULT_SUCCESS;
                            for (int n = 0; n < sock->callbacks.size(); ++n) {
                                if (sock->callbacks[n]->mid == message->mid) {
                                    sock->callbacks[n]->func(message.get());
                                    if (sock->callbacks[n]->remove_after_call) {
                                        sock->callbacks.erase(sock->callbacks.begin() + n);
                                        --n;
                                    }else {
                                        sock->callbacks[n]->reset_timeout();
                                    }
                                }
                            }

                            msg::print_extracted_params(false, true);

							/*if (VALID_PARAMS(mid, _MID->RELAY_TEST)) {
                                int a = msg::last_param_list[0]->get<int>();
                                char* b = msg::last_param_list[1]->get<char*>();
								u_short c = msg::last_param_list[2]->get<u_short>();
								u_short d = msg::last_param_list[3]->get<u_short>();

                                msg::print_extracted_params();

								msg::send(*sock, msg::MsgStream() << _MID->RELAY_TEST << a << msg::last_param_list[1] << c << d);

								std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							}else if (VALID_PARAMS(mid, _MID->RECV_ID)) {
                                msg::print_extracted_params();
                            }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_BIND_REQUEST)) {
                                peers::peer_join(msg::last_param_list[0]->get<int>(), msg::last_param_list[1]->get<char*>());

                                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                            }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_PORT)) {
                                peer = peers::get_peer(msg::last_param_list[0]->get<int>());
                                if (peer != NULL && !peer->connected) {
                                    peer->udp_send_port = msg::last_param_list[1]->get<u_short>();
                                    peer->udp_sock->s_change_send_addr("127.0.0.1", peer->udp_send_port);
                                    server_poll.add_sock(*peer->udp_sock);
                                    peer->connected = true;
                                    msg::send(*peer->udp_sock, msg::MsgStream() << _MID->PO_PING_CONNECT_TEST);

                                    log_info << "sending ping to peer (peer_id: " << peer->id << 
                                                ", peer_recv_port: " << peer->udp_recv_port << ", peer_send_port: " << peer->udp_send_port << ")";
                                }else {
                                    log_warning << "peer " << msg::last_param_list[0]->get<int>() << " could not be found (recv_peer_port)";
                                }
                            }else if (VALID_PARAMS(mid, _MID->PO_PLAYER_MOVEMENT)) {
                                peer = peers::get_peer(*sock);
                                if (peer != NULL) {
                                    entity::test_peer_movement(peer, msg::last_param_list[0]->get<int>(), 
                                                                     msg::last_param_list[1]->get<int>(), 
                                                                     msg::last_param_list[2]->get<float>());
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
							}*/
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