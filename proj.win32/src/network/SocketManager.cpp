#include "SocketManager.h"
#include "../StateManager.h"

using err::fresult;

Socket sock::tcp_serv_sock;
Socket sock::udp_serv_sock;
std::thread sock::msg_recv_thread;
std::thread sock::tcp_connect_thread;

bool sock::udp_ping_pong = false;
float sock::udp_ping_pong_time = 0;
int sock::udp_ping_pong_tries = 0;

bool sock::connection_finished = false;
int sock::connection_error = -1;

char* serv_ip = "192.168.0.11";
u_short serv_port = 4222;

void tcp_connect() {
    using namespace sock;

    connection_finished = false;
    connection_error = NO_ERROR;

    CCLOG("attempt connect on thread...");
    tcp_serv_sock = Socket(PROTO_TCP, serv_ip, serv_port);
    if ((fresult = tcp_serv_sock.s_create()) != NO_ERROR) {
        CCLOG("(tcp_serv_sock): error %d occurred while creating tcp socket", fresult);
        socket_setup_failed(fresult); return;
    }
    if ((fresult = tcp_serv_sock.s_connect()) != NO_ERROR) {
        CCLOG("(tcp_serv_sock): error %d occurred while trying to connect to (ip: %s, port: %d)", fresult, tcp_serv_sock.get_ip(), tcp_serv_sock.get_port());
        socket_setup_failed(fresult); return;
    }

    CCLOG("(tcp_serv_sock): connection successful");

    game_msgs::start_recv_thread();
}

bool sock::setup_udp_sock(u_short udp_recv_port, u_short udp_serv_port) {
    udp_serv_sock = Socket(PROTO_UDP, "0.0.0.0", udp_recv_port);
    if ((fresult = udp_serv_sock.s_create()) != NO_ERROR) {
        CCLOG("(udp_serv_sock): error %d occurred while creating tcp socket", fresult);
        socket_setup_failed(fresult); return false;
    }
    if ((fresult = udp_serv_sock.s_bind()) != NO_ERROR) {
        CCLOG("(udp_serv_sock): error %d occurred while trying to bind to (ip: %s, port: %d)", fresult, udp_serv_sock.get_ip(), udp_serv_sock.get_port());
        socket_setup_failed(fresult); return false;
    }
    udp_serv_sock.s_change_addr(serv_ip, udp_serv_port);

    CCLOG("(udp_serv_sock): creation/binding successful");

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
            send_udp_ping_pong();
            if (udp_ping_pong_tries >= MAX_UDP_PING_PONG_TRIES) {
                CCLOG("udp ping pong timed out");
                socket_setup_failed(ERR_UDP_PING_PONG_FAILED);
                udp_ping_pong = false;
            }
        }
    }
}

void sock::send_udp_ping_pong() {
    if (udp_ping_pong_tries < MAX_UDP_PING_PONG_TRIES) {
        message::send(udp_serv_sock, message::ByteStream() << message::MID_UDP_PING_PONG);
        if (!udp_ping_pong) {
            udp_ping_pong_tries = 0;
            udp_ping_pong = true;
        }
        udp_ping_pong_time = state::time_since_startup;
    }
}

void sock::begin_relay_test(Socket& sock) {
    message::send(sock, message::ByteStream() << message::MID_BEGIN_RELAY_TEST);
}

void sock::init() {
    message::init();
    Socket::init_sockets();
}

void sock::setup_tcp_sock() {
    CCLOG("attempt connect...");
    tcp_connect_thread = std::thread(tcp_connect);
}