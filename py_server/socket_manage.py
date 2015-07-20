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
    tcp_sock.setblocking(0);
    udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
    udp_sock.setblocking(0);

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
        for read_sock in read_list:
            if (read_sock == tcp_sock):
                try:
                    client_sock, addr = tcp_sock.accept();
                    client_sock.setblocking(0);
                    client.handle_join(client_sock, addr[0], addr[1]);
                except:
                    continue;

        c = 0;
        for i in range(0, len(client.clients)):
            client_obj = client.clients[c];
            client_tcp_sock = client_obj.tcp_sock;
            client_dc = False;

            try:
                byte_data = client_tcp_sock.recv(1024);
                if (byte_data.__len__() > 0):
                    server_msgs.got_message(read_sock, client_obj, byte_data);
                else:
                    print("socket has disconnected");
                    del clients[c];
                    client.handle_leave(read_sock);
                    client_dc = True;
            except socket.error as serr:
                if (serr.errno != socket.errno.EWOULDBLOCK):
                    print("tcp socket error occurred. err: %s" % serr.message);

            if not (client_dc):
                try:
                    byte_data, addr = udp_sock.recvfrom(1024);
                    if (byte_data.__len__() > 0):
                        if (client_obj.ip == addr[0] and client_obj.port == addr[1]):
                            server_msgs.got_message(udp_sock, client_obj, byte_data);
                    else:
                        print("socket has disconnected");
                        del clients[c];
                        client.handle_leave(read_sock);
                        client_dc = True;
                except socket.error as serr:
                    if (serr.errno != socket.errno.EWOULDBLOCK):
                        print("tcp socket error occurred. err: %s" % serr.message);

            c += 1;

    s.close();