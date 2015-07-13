import socket;
import select;
import sys;
import server;

tcp_sock = None;

def init():
    global tcp_sock;

    tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

def listen(ip, port):
    global tcp_sock;

    tcp_sock.bind((ip, port));
    tcp_sock.listen(1);
    print("awaiting clients...");

    read_list = [tcp_sock];
    write_list = [];

    while (1):
        can_read_list, can_write_list, err = select.select(read_list, write_list, [], 60000);

        for r in can_read_list:
            if (r == tcp_sock):
                client_sock, addr = tcp_sock.accept();
                read_list.append(client_sock);
                write_list.append(client_sock);

                server.client_accepted(client_sock, addr[0], addr[1]);
            else:
                try:
                    data = r.recv(1024);
                except:
                    print("socket has disconnected");
                    read_list.remove(r);
                    write_list.remove(r);

                    print(str(r.getpeername()));
                    
                    server.client_disconnected(r);
                    if (server.num_clients <= 0): break;
                    continue;

                msg = str(data);
                user_id = msg[0:msg.find("|")];
                print("received from client: " + msg);
                print("sending msg to " + str(len(can_write_list)) + " clients");
                for w in can_write_list:
                    w.send(buffer(msg.encode()));

    s.close();