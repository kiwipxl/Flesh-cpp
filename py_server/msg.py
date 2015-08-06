from __future__ import print_function;
import sys;
import struct;
import socket;
import debug;
import time;
import _MID;
import _FT;
import callback;

byte_buffer = bytearray(1024);
byte_offset = 0;
MSG_HEADER_SIZE = 4;

class Message:

    sock = None;
    client_obj = None;
    mid = None;
    params = [];
    raw_data = "";
    callback_result = callback.CALLBACK_RESULT_UNKNOWN;

    def __init__(self, sock = None, client_obj = None, mid = _MID.UNKNOWN, params = []):
        self.sock = sock;
        self.client_obj = client_obj;
        self.mid = mid;
        self.params = params;
        self.raw_data = "";
        self.callback_result = callback.CALLBACK_RESULT_UNKNOWN;

def build(mid, *params):
    byte_buffer[0:4] = struct.pack(_FT.INT.struct_char, mid.id);
    byte_offset = MSG_HEADER_SIZE;
    i = 0;
    if (params):
        try:
            for param in params:
                t = mid.ft_params[i];
                if (t.struct_char == 's'):
                    s = param + '\0';
                else:
                    s = struct.pack(t.struct_char, param);
                byte_buffer[byte_offset:byte_offset + s.__len__()] = s;
                byte_offset += s.__len__();
                i += 1;
        except:
            params = [];
    return byte_buffer;

def extract_message(sock, client_obj, byte_data):
    mid = extract_mid(byte_data);
    params, err = extract_params(mid, byte_data);
    m = None;
    if (err >= 0):
        m = Message(sock, client_obj, mid, params);
    return m;

def extract_mid(byte_data):
    mid = _MID.UNKNOWN;
    if (len(byte_data) >= MSG_HEADER_SIZE):
        id = struct.unpack(_FT.INT.struct_char, byte_data[0:4])[0];
        if (id >= 0 and id <= len(_MID.vec)):
            mid = _MID.vec[id];
        else:
            debug.log("MID id %d is unknown" % id, debug.P_WARNING);
    return mid;

def extract_params(mid, byte_data):
    params = [];
    if (len(byte_data) - MSG_HEADER_SIZE >= mid.total_param_bytes):
        byte_offset = MSG_HEADER_SIZE;
        for n in range(0, len(mid.ft_params)):
            t = mid.ft_params[n];
            if (t.struct_char == 's'):
                s = "";
                l = 0;
                for c in byte_data[byte_offset:]:
                    l += 1;
                    if (c == '\0'): break;
                    s += c;
                byte_offset += l;
            else:
                s = struct.unpack(t.struct_char, byte_data[byte_offset:byte_offset + t.len])[0];
                byte_offset += t.len;
            params.append(s);
        return (params, 0);
    else:
        debug.log("extract params failed. received msg %s is only %i bytes long when the minimum is %i bytes" %
                  (_MID.names[mid.id], len(byte_data), mid.total_param_bytes + MSG_HEADER_SIZE), debug.P_WARNING);
        return (params, -1);

def send(sock, client_obj, built_msg):
    if (len(built_msg) < MSG_HEADER_SIZE):
        debug.log("built message does not contain a full header", debug.P_WARNING);

    if (sock.type == socket.SOCK_STREAM):
        send_tcp(sock, built_msg);
    else:
        send_udp(sock, client_obj.ip, client_obj.c_udp_port, built_msg);

def send_tcp(tcp_sock, built_msg):
    tcp_sock.send(built_msg);

def send_udp(udp_sock, ip, port, built_msg):
    if (port <= 0): debug.log("port is less than zero when trying to send_udp message", debug.P_WARNING); return;
    udp_sock.sendto(built_msg, (ip, port));

def broadcast(sock_list, client_obj, built_msg):
    for sock in sock_list:
        send(sock, client_obj, built_msg);

def log(message):
    if (message.mid.num_params >= 0 and message.mid.num_params == len(message.params)):
        debug.log("", debug.P_MID, "");
        print(_MID.names[message.mid.id] + " (client_id: %d, %s)" % (message.client_obj.id,
                                                                 "tcp" if message.sock.type == socket.SOCK_STREAM
                                                                 else "udp"), end='');
        if (message.mid.num_params >= 1 and message.params != None):
            print(": ", end='');
            i = 0;
            for param in message.params:
                print(("(%s): %" + message.mid.ft_params[i].printf_char) % (message.mid.ft_params[i].type_name, param), end='');
                i += 1;
                print("") if i >= len(message.params) else print(", ", end='');
        else:
            print("");
    else:
        debug.log("could not print params, required %d params, but %d params given" % (len(message.params), message.mid.num_params), debug.P_WARNING);