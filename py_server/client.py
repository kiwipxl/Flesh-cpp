import message;
import debug;
import game;

class Client:
    id = 0;
    tcp_sock = None;
    udp_sock = None;
    ip = "";
    c_tcp_port = -1;   #binded tcp port on client machine connected to client tcp socket
    c_udp_port = -1;   #binded udp port on client machine that is listening for messages
    s_tcp_port = -1;   #binded tcp port on server machine connected to client tcp socket
    s_udp_port = -1;   #binded udp port on server machine that is listening for messages
    joined_game = None;
    game_client = None;

clients = [];
num_clients = 0;
client_id_inc = 0;

def handle_join(new_tcp_sock, new_udp_sock, client_ip, client_port, add_to_list = True):
    global clients
    global num_clients
    global client_id_inc

    c = Client();
    c.tcp_sock = new_tcp_sock;
    c.udp_sock = new_udp_sock;
    c.id = client_id_inc;
    c.ip = client_ip;
    c.c_tcp_port = client_port;
    c.s_tcp_port = new_tcp_sock.getsockname()[1];
    c.s_udp_port = new_udp_sock.getsockname()[1];

    if (add_to_list): clients.append(c);

    #only accept client when all sockets are verified
    debug.log("accepted client (client-id: %d, ip: %s, c_tcp_port: %d, c_udp_port: %d, s_tcp_port: %d, s_udp_port: %d)" %
          (c.id, c.ip, c.c_tcp_port, c.c_udp_port, c.s_tcp_port, c.s_udp_port), debug.P_INFO);

    num_clients += 1;
    client_id_inc += 1;
    message.send(c.tcp_sock, c, message.MID_SEND_SERVER_BINDED_UDP_PORT, (new_udp_sock.getsockname()[1],));

def handle_leave(client_obj, leave_message, remove_from_list = True):
    global clients
    global num_clients

    client_obj.tcp_sock.close();
    client_obj.udp_sock.close();

    game.client_leave(client_obj);

    debug.log("client disconnected (client-id: %d, ip: %s, c_tcp_port: %d, c_udp_port: %d, s_tcp_port: %d, s_udp_port: %d, msg: %s)" %
          (client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port, client_obj.s_tcp_port, client_obj.s_udp_port, leave_message), debug.P_INFO);

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
