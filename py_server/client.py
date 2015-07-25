import message;

class Client:
    id = 0;
    tcp_sock = None;
    udp_sock = None;
    ip = "";
    tcp_port = 0;
    udp_port = 0;

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
    c.tcp_port = client_port;

    if (add_to_list): clients.append(c);

    #only accept client when all sockets are verified
    print("accepted client (client-id: %d, ip: %s, tcp_port: %d, udp_port: %d)" % (c.id, c.ip, c.tcp_port, -1));

    num_clients += 1;
    client_id_inc += 1;
    message.send(c.tcp_sock, c, message.MID_SEND_SERVER_UDP_PORT, (new_udp_sock.getsockname()[1],));

    for cl in clients:
        message.send(cl.tcp_sock, cl, message.MID_SEND_UDP_PEER_BIND_REQUEST, (cl.tcp_port, cl.udp_port));

def handle_leave(client_obj, leave_message, remove_from_list = True):
    global clients
    global num_clients

    client_obj.tcp_sock.close();
    client_obj.udp_sock.close();

    print("client disconnected (client-id: %d, ip: %s, tcp_port: %d, udp_port: %d, msg: %s)" % (client_obj.id, client_obj.ip, client_obj.tcp_port, client_obj.udp_port, leave_message));
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
