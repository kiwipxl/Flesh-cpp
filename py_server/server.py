#!/usr/bin/python

from __future__ import print_function;
import sys;
import db;
import socket_manage;
import message;
from client import *;
import struct;

clients = [];
num_clients = 0;
client_id_inc = 0;

def client_accepted(client_sock, client_ip, client_port):
    global clients
    global num_clients
    global client_id_inc
    
    c = Client();
    c.sock = client_sock;
    c.id = client_id_inc;
    c.ip = client_ip;
    c.port = client_port;

    clients.append(c);

    print("accepted client (client-id: %d, ip: %s, port: %d)" % (c.id, c.ip, c.port));

    num_clients += 1;
    client_id_inc += 1;
    #message.send(c.sock, message.MID_CLIENT_ID, c.id, "test");

def client_disconnected(sock):
    global clients
    global num_clients

    for c in clients:
        if (c.sock == sock):
            print("client disconnected (client-id: %d, ip: %s, port: %d)" % (c.id, c.ip, c.port));
            clients.remove(c);
            num_clients -= 1;
            break;

def got_message(client_sock, byte_data):
    mid = message.extract_mid(byte_data);
    if (mid != message.MID_UNKNOWN):
        (params, err) = message.extract_params(mid, byte_data);
        if (err != -1):
            if (mid == message.MID_CLIENT_USER_PASS):
                print("username: %s, password: %s" % (params[0], params[1]));
                db.add_user_account(params[0], params[1]);
            elif (mid == message.MID_RELAY_TEST and len(params) == message.MID_RELAY_TEST.num_params):
                print("received relay. sending confirmation back...");
                message.print_params(mid, params);
                message.send(client_sock, message.MID_RELAY_TEST, params[0], params[1], params[2], params[3], params[4]);
    else:
        print("received message (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));

if __name__ == "__main__":
    db.init();
    socket_manage.init();
    socket_manage.listen("0.0.0.0", 4222);