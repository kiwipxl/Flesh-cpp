import sys;

class Enum(tuple): __getattr__ = tuple.index;

ID = Enum(["UNKNOWN", "CLIENT_ID", "CLIENT_USER_PASS"]);

class Message:
    msg_id = ID.UNKNOWN;
    params = [];
    raw_data = "";

b24const = 256 * 256 * 256;
b16const = 256 * 256;

def int2chrstr(num, bits):
    s = "";
    if (bits >= 1): s += chr(num % 256);
    if (bits >= 2): s += chr(num / 256);
    if (bits >= 3): s += chr(num / b16const);
    if (bits >= 4): s += chr(num / b24const);
    return s;

def chrstr2int(chrstr):
    i = 0;
    l = chrstr.__len__();
    if (l >= 4): i += ord(chrstr[3]) * b24const;
    if (l >= 3): i += ord(chrstr[2]) * b16const;
    if (l >= 2): i += ord(chrstr[1]) * 256;
    if (l >= 1): i += ord(chrstr[0]);
    return i;

def make(msg_id, *params):
    m = Message();
    m.msg_id = msg_id;
    m.params = params;
    m.raw_data = encode(msg_id, *params);
    return m;

def encode(msg_id, *params):
    data = int2chrstr(msg_id, 4);
    for param in params:
        s = str(param);
        if (s != ""): data += int2chrstr(s.__len__(), 2) + s;
    return data;

def decode_msg(msg):
    raw_data = msg.raw_data;
    if (len(raw_data) >= 4):
        msg_id = chrstr2int(raw_data[0:3]);
        params = [];
        i = 0;
        start_index = -1;
        slen = raw_data.__len__() + 1;
        index = 4;
        while index < slen:
            if (i == 0):
                if (start_index != -1):
                    params.append(raw_data[start_index:index]);
                if (index + 2 < slen):
                    i = chrstr2int(raw_data[index:index + 2]);
                    start_index = index + 2;
                    index += 2;
            i -= 1;
            index += 1;

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
    sock.send(buffer(encode(msg_id, *params).encode()));

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