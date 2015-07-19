import socket;
import select;
import sys;
import client;
import server_msgs;

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
                read_list.append(client_sock);
                write_list.append(client_sock);

                client.handle_join(client_sock, addr[0], addr[1]);
            else:
                try:
                    byte_data = read_sock.recv(1024);
                    if (byte_data.__len__() <= 0):
                        print("socket has disconnected");
                        read_list.remove(read_sock);

                        client.handle_leave(read_sock);
                        if (client.num_clients <= 0): break;
                except:
                    print("socket has disconnected");
                    read_list.remove(read_sock);

                    client.handle_leave(read_sock);
                    if (client.num_clients <= 0): break;
                    continue;
                server_msgs.got_message(client.find_by_sock(read_sock), byte_data);

    s.close();