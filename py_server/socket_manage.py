import socket;
import select;
import sys;

tcp_sock = None;

def init():
    global tcp_sock;
    tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

def listen(ip, port):
    global tcp_sock;
    tcp_sock.bind((ip, port));
    tcp_sock.listen(5);
    print("awaiting clients...");
    
    read_list = [tcp_sock];
    write_list = [];
    clients = [];
    conn_clients = 0;
    user_id_count = 0;

    while (1):
        can_read_list, can_write_list, err = select.select(read_list, write_list, [], 60000);

        for r in can_read_list:
            if (r == tcp_sock):
                client_s, addr = tcp_sock.accept();
                print("accepted client");
                read_list.append(client_s);
                write_list.append(client_s);

                c = Client();
                c.socket = client_s;
                c.user_id = user_id_count;
                clients.append(c);

                conn_clients += 1;
                user_id_count += 1;
                client_s.send(buffer(("userid:" + str(c.user_id)).encode()));
            else:
                try:
                    data = r.recv(1024);
                except:
                    print("socket has disconnected");
                    read_list.remove(r);
                    write_list.remove(r);

                    for c in clients:
                        if (c.socket == r):
                            clients.remove(c);
                            break;

                    conn_clients -= 1;
                    if (conn_clients <= 0):
                        break;
                    continue;

                msg = str(data);
                user_id = msg[0:msg.find("|")];
                print("received from client: " + msg);
                print("sending msg to " + str(len(can_write_list)) + " clients");
                for w in can_write_list:
                    w.send(buffer(msg.encode()));

    s.close();