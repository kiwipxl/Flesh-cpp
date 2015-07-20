import message;

class Client:
    id = 0;
    tcp_sock = None;
    ip = "";
    port = 0;

clients = [];
num_clients = 0;
client_id_inc = 0;

def handle_join(client_sock, client_ip, client_port, add_to_list = True):
    global clients
    global num_clients
    global client_id_inc

    c = Client();
    c.tcp_sock = client_sock;
    c.id = client_id_inc;
    c.ip = client_ip;
    c.port = client_port;

    if (add_to_list): clients.append(c);

    print("accepted client (client-id: %d, ip: %s, port: %d)" % (c.id, c.ip, c.port));

    num_clients += 1;
    client_id_inc += 1;
    message.send(c.tcp_sock, c, message.MID_GET_TCP_CLIENT_PORT, (client_port,));

def handle_leave(client_obj, leave_message, remove_from_list = True):
    global clients
    global num_clients

    print("client disconnected (client-id: %d, ip: %s, port: %d, leave_msg: %s)" % (client_obj.id, client_obj.ip, client_obj.port, leave_message));
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
        elif (c.ip == addr[0] and c.port == addr[1]):
            return c;
