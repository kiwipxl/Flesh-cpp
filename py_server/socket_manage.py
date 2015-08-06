import socket;
import select;
import sys;
import client;
import server_msgs;
import time;
import debug;
import callback;
import _MID;
import msg;

#used to listen to a port for incoming connections and accepting them
tcp_sock = None;

def init():
    global tcp_sock;

    tcp_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    tcp_sock.setblocking(0);

def socket_loop(listen_ip, listen_port):
    global tcp_sock;

    tcp_sock.bind((listen_ip, listen_port));
    tcp_sock.listen(1);
    debug.log("awaiting clients...", debug.P_INFO);

    while (1):
        try:
            client_sock, addr = tcp_sock.accept();
            client_sock.setblocking(0);
            udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
            udp_sock.bind(("0.0.0.0", 0));
            udp_sock.setblocking(0);
            client.handle_join(client_sock, udp_sock, True);
        except socket.error as serr:
            if (serr.errno != socket.errno.EWOULDBLOCK):
                debug.log("error occurred while accepting client: %s" % serr.strerror, debug.P_ERROR);

        c = 0;
        for i in range(0, len(client.clients)):
            client_obj = client.clients[c];
            client_dc = False;
            sockerr = None;

            callback.process_callbacks();

            try:
                byte_data = client_obj.tcp_sock.recv(1024);
                if (byte_data.__len__() > 0):
                    server_msgs.got_msg(client_obj.tcp_sock, client_obj, byte_data);
                else:
                    client.handle_leave(client_obj, "HOST_QUIT", False);
                    client_dc = True;
            except socket.error as serr:
                if (serr.errno != socket.errno.EWOULDBLOCK):
                    sockerr = serr;

            if not (client_dc):
                try:
                    byte_data, addr = client_obj.udp_sock.recvfrom(1024);
                    if (byte_data.__len__() > 0):
                        server_msgs.got_msg(client_obj.udp_sock, client_obj, byte_data);
                    else:
                        client.handle_leave(client_obj, "HOST_QUIT", False);
                        client_dc = True;
                except socket.error as serr:
                    if (serr.errno != socket.errno.EWOULDBLOCK):
                        sockerr = serr;

            if (client_dc):
                del client.clients[c];
                c -= 1;

            if (sockerr):
                del client.clients[c];
                if (sockerr.errno == socket.errno.ECONNRESET):
                    client.handle_leave(client_obj, "HOST_FORCE_QUIT", False);
                else:
                    errno = sockerr.errno;
                    debug.log("socket error occurred (or not handled for). err: %s" % sockerr.strerror, debug.P_ERROR);
                    client.handle_leave(client_obj, sockerr.strerror, False);
            else:
                c += 1;

    s.close();