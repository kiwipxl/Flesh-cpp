import socket;
import select;
import sys;
import client;
import server_msgs;
import time;

tcp_sock = None;
udp_sock = None;
udp_sock_send = None;

def init():
    global tcp_sock;
    global udp_sock;
    global udp_sock_send;

    tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
    udp_sock_send = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);

def listen(ip, port):
    global tcp_sock;
    global udp_sock;
    global udp_sock_send;

    #tcp_sock.bind((ip, port));
    udp_sock.bind((ip, port));
    #tcp_sock.listen(1);
    print("awaiting clients...");

    read_list = [tcp_sock, udp_sock];
    write_list = [];

    while (1):
        print("receiving...");
        buf, addr = udp_sock.recvfrom(1024);
        print("buf: %s, addr: %s" % (buf, addr));
        udp_sock_send.sendto(buf, (ip, 4224));
        print("received!");

        can_read_list, can_write_list, err = select.select(read_list, write_list, [], 1);

        for read_sock in can_read_list:
            if (read_sock == tcp_sock):
                client_sock, addr = tcp_sock.accept();
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