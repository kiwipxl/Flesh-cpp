#include "SocketManager.h"

using err::fresult;

Socket sock::tcp_serv_sock;
Socket sock::udp_serv_sock;
std::thread sock::msg_recv_thread;
std::thread sock::tcp_connect_thread;

bool sock::done_connecting = false;
int sock::connect_result = -1;

void tcp_connect() {
    using namespace sock;

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

    msg_recv_thread = std::thread(messagerecv::recv_msgs);
}

void sock::setup_udp_sock(u_short udp_recv_port, u_short udp_serv_port) {
    udp_serv_sock = Socket(PROTO_UDP, "0.0.0.0", udp_recv_port);
    if ((fresult = udp_serv_sock.s_create()) != NO_ERROR) {
        CCLOG("(udp_serv_sock): error %d occurred while creating tcp socket", fresult);
        socket_setup_failed(fresult); return;
    }
    if ((fresult = udp_serv_sock.s_bind()) != NO_ERROR) {
        CCLOG("(udp_serv_sock): error %d occurred while trying to bind to (ip: %s, port: %d)", fresult, udp_serv_sock.get_ip(), udp_serv_sock.get_port());
        sock::socket_setup_failed(fresult); return;
    }
    udp_serv_sock.s_change_addr("192.168.0.2", udp_serv_port);
    message::send(&udp_serv_sock, message::ByteStream() << message::MID_BEGIN_RELAY_TEST);
    message::send(&tcp_serv_sock, message::ByteStream() << message::MID_BEGIN_RELAY_TEST);

    sock::done_connecting = true;
    sock::connect_result = NO_ERROR;

    CCLOG("(udp_serv_sock): creation/binding successful");
}

void sock::socket_setup_failed(int err) {
    done_connecting = true;
    connect_result = err;
}

void sock::init() {
    message::init();
    Socket::init_sockets();
}

void sock::setup_tcp_sock() {
    CCLOG("attempt connect...");
    tcp_connect_thread = std::thread(tcp_connect);
}