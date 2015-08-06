#include "SocketManager.h"

#include "../../StateManager.h"
#include "../../debug/Logger.h"
#include "../message/Message.h"
#include "../GameMessages.h"

using debug::fresult;

Socket sock::tcp_serv_sock;
Socket sock::udp_serv_sock;
std::thread sock::tcp_connect_thread;

bool sock::udp_ping_pong = false;
float sock::udp_ping_pong_time = 0;
int sock::udp_ping_pong_tries = 0;
Socket* sock::udp_ping_pong_sock = NULL;

bool sock::connection_finished = false;
int sock::connection_error = -1;

char* serv_ip = "127.0.0.1";
u_short serv_port = 4222;

void tcp_connect() {
    using namespace sock;

    connection_finished = false;
    connection_error = NO_ERROR;
    
    log_info << "attempt connect on thread...";
    if ((fresult = tcp_serv_sock.s_create()) != NO_ERROR) {
        log_error << "(tcp_serv_sock): error " << fresult << " occurred while creating tcp socket";
        socket_setup_failed(fresult); return;
    }
    if ((fresult = tcp_serv_sock.s_connect(serv_ip, serv_port)) != NO_ERROR) {
        log_error << "(tcp_serv_sock): error " << fresult << " occurred while trying to connect to (ip: " <<
                           tcp_serv_sock.get_binded_ip() << ", port: " << tcp_serv_sock.get_binded_port() << ")";
        socket_setup_failed(fresult); return;
    }

    tcp_serv_sock.add_message_handler(msg::MID_REQUEST_CLIENT_TO_BIND_UDP_PORT, [](msg::Message* message) {
        if (setup_udp_sock(message->get<u_short>(0))) {
            msg::game::server_poll.add_sock(sock::udp_serv_sock);
            msg::send(tcp_serv_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << udp_serv_sock.get_binded_port());
        }else {
            msg::send(tcp_serv_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << 0);

            connection_finished = true;
            connection_error = -1;
        }
    });

    /*tcp_serv_sock.add_callback(msg::make_MID_once_callback([]() {
        udp_serv_sock.add_callback(msg::make_MID_callback([]() {
            msg::CallbackFunc cb00;
            cb00 = [&]() {
                msg::ResponseCode rc = msg::last_param_list[0]->get<msg::ResponseCode>();
                if (rc == msg::RESPONSE_SUCCESS) {
                    sock::udp_ping_pong = false;
                    sock::connection_finished = true;
                    sock::connection_error = NO_ERROR;
                }else {
                    sock::udp_ping_pong = false;
                    sock::connection_finished = true;
                    sock::connection_error = NO_ERROR;
                    msg::send(udp_serv_sock, msg::MsgStream() << _MID->UDP_PONG, msg::make_response_callback(cb00));
                }
                return msg::RESPONSE_NONE;
            };

            msg::send(udp_serv_sock, msg::MsgStream() << _MID->UDP_PONG, msg::make_response_callback(cb00));

            return msg::RESPONSE_NONE;
        }, _MID->UDP_PING));

        msg::print_extracted_params();

        if (setup_udp_sock(msg::last_param_list[0]->get<u_short>())) {
            msg::game::server_poll.add_sock(sock::udp_serv_sock);
            msg::send(tcp_serv_sock, msg::MsgStream() << _MID->SEND_CLIENT_BINDED_UDP_PORT << udp_serv_sock.get_binded_port());
        }else {
            msg::send(tcp_serv_sock, msg::MsgStream() << _MID->SEND_CLIENT_BINDED_UDP_PORT << -1);

            connection_finished = true;
            connection_error = -1;
        }
        return msg::RESPONSE_NONE;
    }, _MID->RECV_SERVER_BINDED_UDP_PORT));*/

    log_info << "(tcp_serv_sock): connection successful";

    msg::game::start_recv_thread();
}

bool sock::setup_udp_sock(u_short udp_serv_port) {
    if ((fresult = udp_serv_sock.s_create()) != NO_ERROR) {
        log_error << "(udp_serv_sock): error " << fresult << " occurred while creating tcp socket";
        socket_setup_failed(fresult); return false;
    }
    if ((fresult = udp_serv_sock.s_bind("0.0.0.0", 0)) != NO_ERROR) {
        log_error << "(udp_serv_sock): error " << fresult << " occurred while trying to bind to (ip: " <<
            udp_serv_sock.get_binded_ip() << ", port: " << udp_serv_sock.get_binded_port() << ")";
        socket_setup_failed(fresult); return false;
    }
    udp_serv_sock.s_change_send_addr(serv_ip, udp_serv_port);
    
    log_info << "(udp_serv_sock): creation / binding successful";

    return true;
}

void sock::socket_setup_failed(int err) {
    connection_finished = true;
    connection_error = err;
}

void sock::update() {
    if (udp_ping_pong) {
        if (state::time_since_startup - udp_ping_pong_time >= UDP_PING_PONG_TIMEOUT) {
            ++udp_ping_pong_tries;
            if (udp_ping_pong_sock == NULL) { log_info << "udp ping pong sock is null"; return; }
            send_udp_ping_pong(*udp_ping_pong_sock);
            if (udp_ping_pong_tries >= MAX_UDP_PING_PONG_TRIES) {
                log_info << "udp ping pong timed out";
                socket_setup_failed(ERR_UDP_PING_PONG_FAILED);
                udp_ping_pong = false;
            }
        }
    }
}

void sock::send_udp_ping_pong(Socket& sock) {
    if (udp_ping_pong_tries < MAX_UDP_PING_PONG_TRIES) {
        if (sock.get_protocol() != PROTO_UDP) { log_warning << "cannot send udp ping pong: socket is not a udp socket"; return; }
        //msg::send(sock, msg::Stream() << _MID->UDP_PING);
        if (!udp_ping_pong) {
            udp_ping_pong_tries = 0;
            udp_ping_pong = true;
        }
        udp_ping_pong_time = state::time_since_startup;
        udp_ping_pong_sock = &sock;
    }else {
        log_warning << "udp ping pong tries timed out?!?!?1";
    }
}

void sock::begin_relay_test(Socket& sock) {
    //msg::send(sock, msg::Stream() << _MID->BEGIN_RELAY_TEST);
}

void sock::close_all_threads() {
    tcp_connect_thread.detach();
}

void sock::init() {
    udp_serv_sock = Socket(PROTO_UDP);
    tcp_serv_sock = Socket(PROTO_TCP);
    Socket::init_sockets();
}

void sock::setup_tcp_sock() {
    log_info << "attempt connect...";
    tcp_connect_thread = std::thread(tcp_connect);
}