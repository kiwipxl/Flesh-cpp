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
                message.send(sock, client_obj, message.MID_RELAY_TEST, params[0], params[1], random.randrange(0, 100), params[3], params[4]);
            elif (mid == message.MID_CLIENT_ID and len(params) == message.MID_CLIENT_ID.num_params):
                message.print_params(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.MID_RELAY_TEST, True, False, random.randrange(0, 100), 2.458, "ayyo");
    else:
        print("received message (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));