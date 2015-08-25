#include "network/server/ServerConnection.h"

#include "debug/Logger.h"
#include "network/message/Message.h"
#include "network/GameMessages.h"
#include "network/message/MID.h"
#include "network/message/Stream.h"
#include "network/sockets/Socket.h"
#include "StateManager.h"

BEGIN_NETWORK_NS
BEGIN_SERVER_NS

//private
int result;

//public
sock::Socket tcp_sock;
sock::Socket udp_sock;
std::thread tcp_connect_thread;

bool udp_ping_pong = false;
float udp_ping_pong_time = 0;
int udp_ping_pong_tries = 0;
sock::Socket* udp_ping_pong_sock = NULL;

bool connection_finished = false;
int connection_err = -1;
std::string connection_err_msg = "";

char* SERVER_IP = "104.236.253.123";
char* LOCAL_SERVER_IP = "127.0.0.1";
char* serv_ip = LOCAL_SERVER_IP;
u_short serv_port = 4222;
std::mutex thread_lock;

void tcp_connect() {
    using namespace sock;

    log_info << "attempt connect on thread...";
    if ((result = tcp_sock.s_create()) != NO_ERROR) {
        log_error << "(tcp_sock): error " << result << " occurred while creating tcp socket";
        socket_setup_failed(result); return;
    }
    if ((result = tcp_sock.s_connect(serv_ip, serv_port)) != NO_ERROR) {
        log_error << "(tcp_sock): error " << result << " occurred while trying to connect to (ip: " <<
                            tcp_sock.get_binded_ip() << ", port: " << tcp_sock.get_binded_port() << ")";
        socket_setup_failed(result); return;
    }

    log_info << "(tcp_sock): connection successful";

    connection_finished = true;
    connection_err = NO_ERROR;

    tcp_sock.add_leave_handler([](msg::Message* message) {
        char* leave_msg = message->get<char*>(0);
        connection_finished = true;
        connection_err = ERR_SERVER_LEAVE_CODE;
        connection_err_msg = leave_msg;
    });

    //below is code to setup a udp socket for the server, needs to be moved once the user login system is done
    tcp_sock.add_message_handler(msg::MID_RECV_REQUEST_FOR_ME_TO_BIND_UDP_PORT, [](msg::Message* message) {
        if (setup_udp_sock(message->get<u_short>(0))) {
            msg::add_poll_sock(udp_sock);
            //msg::send(tcp_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << udp_sock.get_binded_port());
            msg::send(tcp_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << 0);
        }else {
            msg::send(tcp_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << 0);
        }
    });

    udp_sock.add_message_handler(msg::MID_RECV_UDP_PING, [](msg::Message* message) {
        msg::send(udp_sock, msg::Stream() << msg::MID_SEND_UDP_PONG);
    });

    tcp_sock.add_message_handler(msg::MID_RECV_SERVER_CONNECTION_ESTABLISHED_SUCCESSFULLY, [](msg::Message* message) {
        connection_finished = true;
        connection_err = NO_ERROR;
    });

    msg::start_recv_thread();
}

bool setup_udp_sock(u_short udp_port) {
    if ((result = udp_sock.s_create()) != NO_ERROR) {
        log_error << "(udp_sock): error " << result << " occurred while creating tcp socket";
        socket_setup_failed(result); return false;
    }
    if ((result = udp_sock.s_bind("0.0.0.0", 0)) != NO_ERROR) {
        log_error << "(udp_sock): error " << result << " occurred while trying to bind to (ip: " <<
            udp_sock.get_binded_ip() << ", port: " << udp_sock.get_binded_port() << ")";
        socket_setup_failed(result); return false;
    }
    udp_sock.s_change_send_addr(serv_ip, udp_port);
    
    log_info << "(udp_sock): creation / binding successful";

    return true;
}

void socket_setup_failed(int err) {
    connection_finished = true;
    connection_err = err;
}

void close_all_threads() {
    //todo: close threads properly!
    //if (tcp_connect_thread.native_handle() != NULL) tcp_connect_thread.detach();
    //msg::close_all_threads();
}

void init() {
    udp_sock = sock::Socket(sock::PROTO_UDP);
    tcp_sock = sock::Socket(sock::PROTO_TCP);
    sock::Socket::init_sockets();
}

void cleanup_all() {
    close_all_threads();
    tcp_sock.cleanup();
    udp_sock.cleanup();
}

void setup_tcp_sock() {
    connection_finished = false;
    connection_err = NO_ERROR;

    tcp_connect_thread = std::thread(tcp_connect);
    tcp_connect_thread.detach();
}

END_SERVER_NS
END_NETWORK_NS
