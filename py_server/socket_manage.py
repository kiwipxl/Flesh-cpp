import socket;
import select;
import sys;
import server;
import message;

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
        can_read_list, can_write_list, err = select.select(read_list, write_list, [], 1);

        for read_sock in can_read_list:
            if (read_sock == tcp_sock):
                client_sock, addr = tcp_sock.accept();
                read_list.append(client_sock);
                write_list.append(client_sock);

                server.client_accepted(client_sock, addr[0], addr[1]);
            else:
                try:
                    byte_data = read_sock.recv(1024);
                except:
                    print("socket has disconnected");
                    read_list.remove(read_sock);
                    write_list.remove(read_sock);

                    server.client_disconnected(read_sock);
                    if (server.num_clients <= 0): break;
                    continue;
                server.got_message(byte_data);

    s.close();