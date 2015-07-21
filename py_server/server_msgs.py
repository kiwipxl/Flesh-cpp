import sys;
import db;
import socket_manage;
import message;
from client import *;
import time;
import random;

def verify_params(recv_mid, mid, np):
    return recv_mid == mid and np == mid.num_params;

def got_message(sock, client_obj, byte_data):
    mid = message.extract_mid(byte_data);
    if (mid != message.MID_UNKNOWN):
        (params, err) = message.extract_params(mid, byte_data);
        if (err != -1):
            np = len(params);
            if (verify_params(mid, message.MID_CLIENT_USER_PASS, np)):
                print("username: %s, password: %s" % (params[0], params[1]));
                db.add_user_account(params[0], params[1]);

            elif (verify_params(mid, message.MID_RELAY_TEST, np)):
                message.print_params(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.MID_RELAY_TEST, (client_obj.id, client_obj.ip, client_obj.tcp_port, client_obj.udp_port));

            elif (verify_params(mid, message.MID_CLIENT_ID, np)):
                message.print_params(client_obj, sock.type, mid, params);

            elif (verify_params(mid, message.MID_GET_TCP_AND_UDP_CLIENT_PORTS, np)):
                message.print_params(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.MID_GET_TCP_CLIENT_PORT, (client_obj.tcp_port, client_obj.udp_port,));

            elif (verify_params(mid, message.MID_BEGIN_RELAY_TEST, np)):
                message.send(sock, client_obj, message.MID_RELAY_TEST, (client_obj.id, client_obj.ip, client_obj.tcp_port, client_obj.udp_port));

            elif (verify_params(mid, message.MID_UDP_PING_PONG, np)):
                message.send(sock, client_obj, message.MID_UDP_PING_PONG);
    else:
        print("received message (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));