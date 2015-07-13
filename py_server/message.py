import sys;

class Enum(tuple): __getattr__ = tuple.index;

ID = Enum(["UNKNOWN", "CLIENT_ID", "SEND_USER_PASS"]);

class Message:
    msg_id = ID.UNKNOWN;

def send(sock, msg_id, *params):
    data = str(msg_id);
    for param in params:
        s = str(param);
        data += str(s.__len__()) + s;
    sock.send(buffer(data.encode()));

def broadcast(sock_list, msg_id, *params):
    data = str(msg_id);
    for param in params:
        s = str(param);
        data += str(s.__len__()) + s;
    for sock in sock_list:
        sock.send(buffer(data.encode()));