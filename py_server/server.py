#!/usr/bin/python

import sys;
import db;
import socket_manage;
import message;
from client import *;

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
    client_sock.send(buffer(("userid:" + str(c.id)).encode()));

def client_disconnected(r):
    global clients
    global num_clients

    for c in clients:
        if (c.socket == r):
            clients.remove(c);
            break;

    num_clients -= 1;

if __name__ == "__main__":
    db.init();
    socket_manage.init();
    socket_manage.listen("0.0.0.0", 4222);