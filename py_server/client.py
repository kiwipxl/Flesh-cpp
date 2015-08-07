import msg;
import debug;
import game;
import _MID;
import callback;

class Client:
    id = 0;
    tcp_sock = None;
    udp_sock = None;
    ip = "";
    c_tcp_port = -1;   #binded tcp port on client machine connected to client tcp socket
    c_udp_port = -1;   #binded udp port on client machine that is listening for msgs
    s_tcp_port = -1;   #binded tcp port on server machine connected to client tcp socket
    s_udp_port = -1;   #binded udp port on server machine that is listening for msgs
    joined_game = None;
    game_client = None;
    callbacks = [];
    left = False;

    udp_timeout_tries = 0;

    def __init__(self):
        self.udp_timeout_tries = 0;

    def add_callback(self, callback_obj):
        if (callback_obj):
            self.callbacks.append(callback_obj);

    def add_message_handler(self, mid, func, timeout_len = callback.TIMEOUT_NONE, remove_after_call = False):
        self.callbacks.append(callback.make_MID_callback(mid, func, timeout_len, remove_after_call));

clients = [];
num_clients = 0;
client_id_inc = 0;

def handle_join(new_tcp_sock, new_udp_sock, add_to_list = True):
    global clients
    global num_clients
    global client_id_inc

    c = Client();
    c.tcp_sock = new_tcp_sock;
    c.udp_sock = new_udp_sock;
    c.id = client_id_inc;
    c.ip = new_tcp_sock.getpeername()[0];
    c.c_tcp_port = new_tcp_sock.getpeername()[1];
    c.s_tcp_port = new_tcp_sock.getsockname()[1];
    c.s_udp_port = new_udp_sock.getsockname()[1];
    c.callbacks = [];

    if (add_to_list): clients.append(c);
    
    #only accept client when all sockets are verified
    debug.log("accepted client (client-id: %d, ip: %s, c_tcp_port: %d, c_udp_port: %d, s_tcp_port: %d, s_udp_port: %d)" %
          (c.id, c.ip, c.c_tcp_port, c.c_udp_port, c.s_tcp_port, c.s_udp_port), debug.P_INFO);

    num_clients += 1;
    client_id_inc += 1;

    #c.add_message_handler(_MID.ALL, handle_setup_messages);
    #msg.send(c.tcp_sock, c, msg.build(_MID.REQUEST_CLIENT_TO_BIND_UDP_PORT, new_udp_sock.getsockname()[1]));

def handle_udp_pong(message):
    if (message.callback_result == callback.CALLBACK_RESULT_TIMEOUT):
        message.client_obj.udp_timeout_tries += 1;
        if (message.client_obj.udp_timeout_tries >= 3):
            handle_leave(message.client_obj, "UDP ping pong timed out 3 times");
        else:
            send_udp_ping(message.client_obj);
    else:
        print("udp ping pong success");
        message.client_obj.udp_timeout_tries = 0;
        msg.send(message.client_obj.tcp_sock, message.client_obj,
                 msg.build(_MID.SEND_SERVER_CONNECTION_ESTABLISHED_SUCCESSFULLY));
        #lobby.join_game(client_obj);

def send_udp_ping(client_obj):
    client_obj.add_message_handler(_MID.RECV_UDP_PONG, handle_udp_pong, callback.TIMEOUT_SHORT, True);
    msg.send(client_obj.udp_sock, client_obj, msg.build(_MID.SEND_UDP_PING));

def handle_setup_messages(message):
    if (message.mid == _MID.RECV_CLIENT_BINDED_UDP_PORT):
        if (message.params[0] == 0):
            handle_leave(message.client_obj, "client didn't bind udp port successfully");
        else:
            message.client_obj.c_udp_port = message.params[0];
            send_udp_ping(message.client_obj);

    elif (message.mid == _MID.RECV_UDP_PONG):
        pass;

def handle_leave(client_obj, leave_msg, remove_from_list = True):
    global clients;
    global num_clients;

    client_obj.left = True;
    msg.send(client_obj.tcp_sock, client_obj, msg.build(_MID.SEND_CLIENT_LEAVE, leave_msg));

    game.client_leave(client_obj);

    client_obj.tcp_sock.close();
    client_obj.udp_sock.close();
    client_obj.callbacks = [];

    debug.log("client disconnected (client-id: %d, ip: %s, c_tcp_port: %d, c_udp_port: %d, s_tcp_port: %d, s_udp_port: %d, msg: %s)" %
          (client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port, client_obj.s_tcp_port, client_obj.s_udp_port, leave_msg), debug.P_INFO);

    if (remove_from_list): clients.remove(client_obj);
    client_obj = None;
    num_clients -= 1;

def find_by_sock(sock, addr):
    global clients

    if not (addr): return None;
    if not (sock): return None;

    for c in clients:
        if (read_sock.type == socket.SOCK_STREAM):
            if (c.tcp_sock == sock):
                return c;
        elif (c.udp_sock == sock):
            return c;
