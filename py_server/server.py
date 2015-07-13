#!/usr/bin/python

import socket;
import select;
import sys;
import db;

class Enum(tuple): __getattr__ = tuple.index

if (sys.version_info > (3,)):
    buffer = memoryview
    
class Client:
    user_id = 0;
    socket = None;

MSG_ID = Enum(["UNKNOWN", "USER_ID", "SEND_USER_PASS"]);

class Message:
    msg_id = MSG_ID.UNKNOWN;

msg = Message();
print(str(msg.msg_id) + ", " + str(MSG_ID[msg.msg_id]));

server_s = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
server_s.bind(("0.0.0.0", 4222));
server_s.listen(5);
print("listening...");

read_list = [server_s];
write_list = [];
clients = [];
conn_clients = 0;
user_id_count = 0;

while (1):
    can_read_list, can_write_list, err = select.select(read_list, write_list, [], 60000);

    for r in can_read_list:
        if (r == server_s):
            client_s, addr = server_s.accept();
            print("accepted client");
            read_list.append(client_s);
            write_list.append(client_s);
            
            c = Client();
            c.socket = client_s;
            c.user_id = user_id_count;
            clients.append(c);
            
            conn_clients += 1;
            user_id_count += 1;
            client_s.send(buffer(("userid:" + str(c.user_id)).encode()));
        else:
            try:
                data = r.recv(1024);
            except:
                print("socket has disconnected");
                read_list.remove(r);
                write_list.remove(r);

                for c in clients:
                    if (c.socket == r):
                        clients.remove(c);
                        break;
                    
                conn_clients -= 1;
                if (conn_clients <= 0):
                    break;
                continue;
            
            msg = str(data);
            user_id = msg[0:msg.find("|")];
            print("received from client: " + msg);
            print("sending msg to " + str(len(can_write_list)) + " clients");
            for w in can_write_list:
                w.send(buffer(msg.encode()));
            
s.close();
