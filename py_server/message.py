import sys;

class Enum(tuple): __getattr__ = tuple.index;

ID = Enum(["UNKNOWN", "CLIENT_ID", "CLIENT_USER_PASS"]);

class Message:
    msg_id = ID.UNKNOWN;
    params = [];
    raw_data = "";

def make(msg_id, *params):
    m = Message();
    m.msg_id = msg_id;
    m.params = params;
    m.raw_data = encode(msg_id, params);
    return m;

def encode(msg_id, *params):
    data = str(msg_id);
    for param in params:
        s = str(param);
        data += str(s.__len__()) + s;
    return data;

def decode_msg(msg):
    raw_data = msg.raw_data;
    if (len(raw_data) >= 1 and raw_data[0].isdigit()):
        msg_id = int(raw_data[0]);
        params = [];
        if (len(raw_data) >= 3):
            p = "";
            i = 0;
            start_index = -1;
            for c in raw_data[1:]:
                if (i == 0):
                    if (start_index != -1):
                        params.append(raw_data[start_index:start_index + i]);
                    if (c.isdigit()):
                        i = int(c);
                        start_index = c;
                    else:
                        break;

        msg.msg_id = msg_id;
        msg.params = params;
        return msg;

    msg.msg_id = ID.UNKNOWN;
    return msg;

def decode_raw(raw_data):
    m = Message();
    m.raw_data = raw_data;
    return decode_msg(m);

def send(sock, msg_id, *params):
    sock.send(buffer(encode(msg_id, params).encode()));

def send_msg(sock, msg):
    sock.send(buffer(msg.raw_data.encode()));

def send_raw(sock, raw_data):
    sock.send(buffer(raw_data.encode()));

def broadcast_msg(sock_list, msg):
    for sock in sock_list:
        sock.send(buffer(msg.raw_data.encode()));

def broadcast_raw(sock_list, msg_id, *params):
    for sock in sock_list:
        sock.send(buffer(encode(msg_id, params).encode()));

def broadcast_raw(sock_list, raw_data):
    for sock in sock_list:
        sock.send(buffer(raw_data.encode()));