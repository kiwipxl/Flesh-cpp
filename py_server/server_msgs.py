import sys;
import db;
import socket_manage;
import message;
from client import *;
import time;
import random;

def got_message(sock, client_obj, byte_data):
    mid = message.extract_mid(byte_data);
    if (mid != message.MID_UNKNOWN):
        (params, err) = message.extract_params(mid, byte_data);
        if (err != -1):
            if (mid == message.MID_CLIENT_USER_PASS):
                print("username: %s, password: %s" % (params[0], params[1]));
                db.add_user_account(params[0], params[1]);

            elif (mid == message.MID_RELAY_TEST and len(params) == message.MID_RELAY_TEST.num_params):
                message.print_params(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.MID_RELAY_TEST, (client_obj.id, client_obj.ip, client_obj.port,));

            elif (mid == message.MID_CLIENT_ID and len(params) == message.MID_CLIENT_ID.num_params):
                message.print_params(client_obj, sock.type, mid, params);

            elif (mid == message.MID_GET_TCP_CLIENT_PORT and len(params) == message.MID_GET_TCP_CLIENT_PORT.num_params):
                message.print_params(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.MID_GET_TCP_CLIENT_PORT, (client_obj.port,));

            elif (mid == message.MID_BEGIN_RELAY_TEST and len(params) == message.MID_BEGIN_RELAY_TEST.num_params):
                message.send(sock, client_obj, message.MID_RELAY_TEST, (client_obj.id, client_obj.ip, client_obj.port,));
    else:
        print("received message (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));