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

    def add_callback(self, callback_obj):
        if (callback_obj):
            self.callbacks.append(callback_obj);

    def add_message_handler(self, mid, func):
        self.callbacks.append(callback.make_MID_callback(mid, func));

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

    def cb02(message):
        game.join_game(client_obj);

    c.add_message_handler(_MID.UDP_PONG, cb02);

    def cb00(message):
        message.client_obj.c_udp_port = message.params[0];
        msg.send(message.client_obj.udp_sock, message.client_obj, msg.build(_MID.UDP_PING));

    c.add_message_handler(_MID.RECV_CLIENT_BINDED_UDP_PORT, cb00);

    #msg.send(c.tcp_sock, c, msg.build(_MID.REQUEST_CLIENT_TO_BIND_UDP_PORT, new_udp_sock.getsockname()[1]));

def handle_leave(client_obj, leave_msg, remove_from_list = True):
    global clients;
    global num_clients;

    client_obj.tcp_sock.close();
    client_obj.udp_sock.close();
    client_obj.callbacks = [];

    game.client_leave(client_obj);

    debug.log("client disconnected (client-id: %d, ip: %s, c_tcp_port: %d, c_udp_port: %d, s_tcp_port: %d, s_udp_port: %d, msg: %s)" %
          (client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port, client_obj.s_tcp_port, client_obj.s_udp_port, leave_msg), debug.P_INFO);

    if (remove_from_list): clients.remove(client_obj);
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
