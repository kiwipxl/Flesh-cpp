#include "network/server/ServerConnection.h"

#include "debug/Logger.h"
#include "input/KeyboardInput.h"
#include "gui/MessageBox.h"
#include "network/GameMessages.h"
#include "network/message/Message.h"
#include "network/message/MID.h"
#include "network/message/Stream.h"
#include "network/sockets/Socket.h"
#include "StateManager.h"

BEGIN_NETWORK_NS
BEGIN_SERVER_NS

//private
int result;
bool connected = false;

char* SERVER_IP = "104.236.253.123";
char* LOCAL_SERVER_IP = "127.0.0.1";

void connect_done(ServerConnectCallback _callback, int err, std::string err_msg = "") {
    if (_callback) _callback(err, err_msg);
    connected = (err == NO_ERROR);
}

//public
sock::Socket tcp_sock;
sock::Socket udp_sock;
std::thread tcp_connect_thread;

char* server_ip = LOCAL_SERVER_IP;
u_short server_tcp_port = 4222;
u_short server_udp_port = 0;

bool stop_tcp_thread = false;
bool stop_udp_thread = false;

std::mutex tcp_mutex;
std::mutex udp_mutex;

void init() {
    udp_sock = sock::Socket(sock::PROTO_UDP);
    tcp_sock = sock::Socket(sock::PROTO_TCP);
    sock::Socket::init_sockets();
}

void close_all_threads() {
    //todo: close threads properly!
    //msg::close_all_threads();
    stop_tcp_thread = true;
    stop_udp_thread = true;
}

void cleanup_all() {
    close_all_threads();
    tcp_sock.cleanup();
    udp_sock.cleanup();
}

void setup_tcp_sock(ServerConnectCallback _callback) {
    tcp_connect_thread = std::thread([_callback]() {
        tcp_mutex.lock();

        stop_tcp_thread = false;
        connected = false;

        log_info << "attempt connect on thread...";
        if ((result = tcp_sock.s_create()) != NO_ERROR) {
            log_error << "(tcp_sock): error " << result << " occurred while creating tcp socket";

            tcp_mutex.unlock();
            if (stop_tcp_thread) return;

            connect_done(_callback, result, "could not create tcp socket");
            return;
        }
        if ((result = tcp_sock.s_connect(server_ip, server_tcp_port)) != NO_ERROR) {
            log_error << "(tcp_sock): error " << result << 
                         " occurred while trying to connect to tcp socket (ip: " <<
                         tcp_sock.get_binded_ip() << ", port: " << tcp_sock.get_binded_port() << ")";

            tcp_mutex.unlock();
            if (stop_tcp_thread) return;

            connect_done(_callback, result, "could not connect to tcp socket");
            return;
        }

        log_info << "(tcp_sock): connection successful";

        tcp_mutex.unlock();

        tcp_sock.add_leave_handler([&](msg::Message* message) {
            char* leave_msg = message->get<char*>(0);
            connect_done(_callback, ERR_SERVER_LEAVE_CODE, leave_msg);
        });

        //below is code to setup a udp socket for the server, needs to be moved once the user login system is done
        tcp_sock.add_message_handler(msg::MID_RECV_REQUEST_FOR_ME_TO_BIND_UDP_PORT, [](msg::Message* message) {
            setup_udp_sock(message->get<u_short>(0), [](int err, std::string err_msg) {
                if (err == NO_ERROR) {
                    msg::add_poll_sock(udp_sock);
                    msg::send(tcp_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << udp_sock.get_binded_port());
                }else {
                    msg::send(tcp_sock, msg::Stream() << msg::MID_SEND_CLIENT_BINDED_UDP_PORT << 0);
                }
            });
        });

        udp_sock.add_message_handler(msg::MID_RECV_UDP_PING, [](msg::Message* message) {
            msg::send(udp_sock, msg::Stream() << msg::MID_SEND_UDP_PONG);
        });

        tcp_sock.add_message_handler(msg::MID_RECV_SERVER_CONNECTION_ESTABLISHED_SUCCESSFULLY, [_callback](msg::Message* message) {
            connect_done(_callback, NO_ERROR);
        });

        msg::start_recv_thread();
    });
    tcp_connect_thread.detach();
}

void setup_udp_sock(u_short _server_udp_port, ServerConnectCallback _callback) {
    if ((result = udp_sock.s_create()) != NO_ERROR) {
        log_error << "(udp_sock): error " << result << " occurred while creating udp socket";
        connect_done(_callback, result, "could not create udp socket"); return;
    }
    if ((result = udp_sock.s_bind("0.0.0.0", 0)) != NO_ERROR) {
        log_error << "(udp_sock): error " << result << 
                     " occurred while trying to bind udp socket (ip: " << udp_sock.get_binded_ip() << ")";
        connect_done(_callback, result, "could not bind udp socket"); return;
    }
    udp_sock.s_change_send_addr(server_ip, server_tcp_port);
    
    log_info << "(udp_sock): creation / binding successful";

    connect_done(_callback, NO_ERROR);

    return;
}

bool is_connected() { return connected; }

void update() {
    //toggles local and server ip connections
    //when toggled, the state is switched back to the beginning
    if (input::key_down(input::cc::EventKeyboard::KeyCode::KEY_LEFT_CTRL) && input::key_pressed(input::cc::EventKeyboard::KeyCode::KEY_L)) {
        static bool local_ip = false;
        if (local_ip = !local_ip) {
            log_info << "switched to local server ip";
            server_ip = LOCAL_SERVER_IP;
            cleanup_all();
            root::switch_state(root::STATE_SERVER_CONNECT_SCREEN, true);
            gui::show_message_box("", "switched to local server ip", "OK");
        }else {
            log_info << "switched to server ip";
            server_ip = SERVER_IP;
            cleanup_all();
            root::switch_state(root::STATE_SERVER_CONNECT_SCREEN, true);
            gui::show_message_box("", "switched to server ip", "OK");
        }
    }
}

END_SERVER_NS
END_NETWORK_NS
