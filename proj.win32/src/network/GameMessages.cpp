#include "network/GameMessages.h"

#include "debug/Logger.h"
#include "entities/units/Unit.h"
#include "message/Message.h"
#include "network/Peers.h"
#include "network/message/MID.h"
#include "sockets/SocketManager.h"
#include "StateManager.h"

BEGIN_NETWORK_NS

namespace msg {

    using debug::fresult;

    std::thread msgs_thread;
    sock::SocketPoll server_poll;
    bool end_thread = false;

    #define VALID_PARAMS(a, b) a == b && last_param_list_size >= b->num_params

    void recv_msgs() {
        server_poll.clear();
        server_poll.add_sock(sock::tcp_serv_sock);

	    char buffer[1024];
	    int msg_len;
	    while (true) {
            if (end_thread) break;

            process_all_callbacks(server_poll);

		    int total = 0;
		    if ((total = server_poll.poll()) > 0) {
                int size = server_poll.get_size();
                for (int i = 0; i < size; ++i) {
                    if (server_poll.get_size() != size) break;
                    int revents = server_poll.get_fd_at(i)->revents;
				    if (revents & POLLERR) {
                        log_warning << "poll error occurred";
				    }else if (revents & POLLHUP) {
                        log_warning << "poll hang up error occurred";
				    }else if (revents & POLLNVAL) {
                        log_warning << "poll invalid request occurred";
				    }else if (revents & POLLRDNORM || revents & POLLRDBAND || revents & POLLIN || revents & POLLPRI) {
                        if (server_poll.get_size() != size) break;
                        sock::Socket* sock = server_poll.get_sock_at(i);
                        if (!sock) continue;
                        peers::Peer* peer;

					    if ((msg_len = sock->s_recv(buffer, 1024)) > 0) {
                            MessagePtr message = extract_message(*sock, buffer, msg_len);
						    if (message->mid->id != MID_UNKNOWN) {
                                process_message_callback(message);

                                print_extracted_params(false, true);

							    /*if (VALID_PARAMS(mid, _MID->RELAY_TEST)) {
                                    int a = last_param_list[0]->get<int>();
                                    char* b = last_param_list[1]->get<char*>();
								    u_short c = last_param_list[2]->get<u_short>();
								    u_short d = last_param_list[3]->get<u_short>();

                                    print_extracted_params();

								    send(*sock, MsgStream() << _MID->RELAY_TEST << a << last_param_list[1] << c << d);

								    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							    }else if (VALID_PARAMS(mid, _MID->RECV_ID)) {
                                    print_extracted_params();
                                }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_BIND_REQUEST)) {
                                    peers::peer_join(last_param_list[0]->get<int>(), last_param_list[1]->get<char*>());

                                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                                }else if (VALID_PARAMS(mid, _MID->RECV_UDP_PEER_PORT)) {
                                    peer = peers::get_peer(last_param_list[0]->get<int>());
                                    if (peer != NULL && !peer->connected) {
                                        peer->udp_send_port = last_param_list[1]->get<u_short>();
                                        peer->udp_sock->s_change_send_addr("127.0.0.1", peer->udp_send_port);
                                        server_poll.add_sock(*peer->udp_sock);
                                        peer->connected = true;
                                        send(*peer->udp_sock, MsgStream() << _MID->PO_PING_CONNECT_TEST);

                                        log_info << "sending ping to peer (peer_id: " << peer->id << 
                                                    ", peer_recv_port: " << peer->udp_recv_port << ", peer_send_port: " << peer->udp_send_port << ")";
                                    }else {
                                        log_warning << "peer " << last_param_list[0]->get<int>() << " could not be found (recv_peer_port)";
                                    }
                                }else if (VALID_PARAMS(mid, _MID->PO_PLAYER_MOVEMENT)) {
                                    peer = peers::get_peer(*sock);
                                    if (peer != NULL) {
                                        entity::test_peer_movement(peer, last_param_list[0]->get<int>(), 
                                                                            last_param_list[1]->get<int>(), 
                                                                            last_param_list[2]->get<float>());
                                    }
                                }else if (VALID_PARAMS(mid, _MID->PO_PING_CONNECT_TEST)) {
                                    send(*sock, MsgStream() << _MID->PO_PONG_CONNECT_TEST);
                                }else if (VALID_PARAMS(mid, _MID->PO_PONG_CONNECT_TEST)) {
                                    peer = peers::get_peer(*sock);
                                    if (peer != NULL) {
                                        send(sock::tcp_serv_sock, MsgStream() << _MID->SEND_PEER_CONNECT_SUCCESS << peer->id << peer->ip);
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
                if (end_thread) break;
                log_error << "polling error occurred: " << debug::get_last_error();
		    }
	    }
    }

    void close_all_threads() {
        end_thread = true;
        if (msgs_thread.native_handle() != NULL) msgs_thread.detach();
        server_poll.clear();
    }

    void start_recv_thread() {
        end_thread = false;
        msgs_thread = std::thread(recv_msgs);
        msgs_thread.detach();
    }
};

END_NETWORK_NS