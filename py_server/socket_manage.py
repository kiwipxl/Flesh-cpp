import socket;
import select;
import sys;
import client;
import server_msgs;
import time;

#used to listen to a port for incoming connections and accepting them
tcp_sock = None;
udp_sock = None;

def init():
    global tcp_sock;
    global udp_sock;
    
    tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
    
def listen(ip, port):
    global tcp_sock;
    global udp_sock;
    
    tcp_sock.bind((ip, port));
    udp_sock.bind((ip, port));
    tcp_sock.listen(1);
    print("awaiting clients...");

    read_list = [tcp_sock, udp_sock];
    write_list = [];

    while (1):
        can_read_list, can_write_list, err = select.select(read_list, write_list, [], 0);

        for read_sock in can_read_list:
            if (read_sock == tcp_sock):
                client_sock, addr = tcp_sock.accept();
                read_list.append(client_sock);
                write_list.append(client_sock);

                client.handle_join(client_sock, addr[0], addr[1]);
            else:
                addr = None;
                try:
                    if (read_sock.type == socket.SOCK_STREAM):
                        byte_data = read_sock.recv(1024);
                        addr = read_sock.getsockname();
                    else:
                        byte_data, addr = read_sock.recvfrom(1024);

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
                if (addr):
                    client_obj = None;
                    for c in client.clients:
                        if (read_sock.type == socket.SOCK_STREAM):
                            if (c.sock == read_sock):
                                client_obj = c;
                        elif (c.ip == addr[0] and c.port == addr[1]):
                            client_obj = c;

                    if (client_obj):
                        server_msgs.got_message(client_obj, byte_data);

    s.close();