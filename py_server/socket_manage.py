import socket;
import select;
import sys;
import server;
import message;
import time;

send_sock = None;

def init():
    global send_sock;

    send_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);

def listen(ip, port):
    global send_sock;

    send_sock.bind((ip, port));
    send_sock.listen(1);
    print("awaiting clients...");

    read_list = [send_sock];
    write_list = [];

    while (1):
        can_read_list, can_write_list, err = select.select(read_list, write_list, [], 1);

        for read_sock in can_read_list:
            if (read_sock == send_sock):
                client_sock, addr = send_sock.accept();
                time.sleep(2);
                print("not asleep anymore");
                recv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
                recv_sock.connect((addr[0], 4223));
                client_sock = recv_sock;
                client_sock.send("ayyyyyyyyyyyyyyy");
                print("sent ayyyyyyyyy");
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