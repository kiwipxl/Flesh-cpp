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
    if (l >= 4): i += int(chrstr[3]) * b24const;
    if (l >= 3): i += int(chrstr[2]) * b16const;
    if (l >= 2): i += int(chrstr[1]) * 256;
    if (l >= 1): i += int(chrstr[0]);
    return s;

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
        data += int2chrstr(s.__len__(), 2) + s;
    return data;

def decode_msg(msg):
    raw_data = msg.raw_data;
    print(raw_data);
    if (len(raw_data) >= 1 and raw_data[0].isdigit()):
        msg_id = int(raw_data[0]);
        params = [];
        if (len(raw_data) >= 3):
            print("ayy");
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