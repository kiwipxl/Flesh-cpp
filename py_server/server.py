#!/usr/bin/python

import sys;
import db;
import socket_manage;

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

if __name__ == "__main__":
    db.init();
    socket_manage.init();
    socket_manage.listen("0.0.0.0", 4222);