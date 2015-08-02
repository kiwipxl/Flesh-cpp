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
MSG_HEADER_SIZE = 8;

def build(mid, params = None):
    byte_offset = 4;
    i = 0;
    byte_buffer[0:4] = struct.pack(_FT.INT.struct_char, mid.id);
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

def extract_mid(byte_data):
    mid = _MID.UNKNOWN;
    if (len(byte_data) >= MSG_HEADER_SIZE):
        id = struct.unpack("i", byte_data[0:4])[0];
        if (id >= 0 and id <= len(_MID.list)):
            mid = _MID.list[id];
        else:
            debug.log("MID id %d is unknown" % id, debug.P_WARNING);
    return mid;

def extract_callback(byte_data):
    callback_id = 0;
    if (len(byte_data) >= MSG_HEADER_SIZE):
        callback_id = struct.unpack("i", byte_data[4:8])[0];
    return callback_id;

def extract_params(mid, byte_data):
    params = [];
    if (len(byte_data) - 4 >= mid.total_param_bytes):
        byte_offset = 4;
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
                  (_MID.names[mid.id], len(byte_data), mid.total_param_bytes + 4), debug.P_WARNING);
        return (params, -1);

def send(sock, client_obj, built_msg, callback_obj = None):
    if (len(built_msg) < MSG_HEADER_SIZE):
        debug.log("built message does not contain a full header", debug.P_WARNING);

    if (sock.type == socket.SOCK_STREAM):
        send_tcp(sock, built_msg);
    else:
        send_udp(sock, client_obj.ip, client_obj.c_udp_port);
    client_obj.add_callback(callback_obj);

def send_tcp(tcp_sock, built_msg):
    tcp_sock.send(built_msg);

def send_udp(udp_sock, ip, port, built_msg):
    if (port <= 0): debug.log("port is less than zero when trying to send_udp message", debug.P_WARNING); return;
    udp_sock.sendto(built_msg, (ip, port));

def broadcast(sock_list, client_obj, built_msg):
    for sock in sock_list:
        send(sock, client_obj, built_msg);

def log(client_obj, sock_type, mid, params = None):
    if (params == None or (mid.num_params >= 0 and mid.num_params == len(params))):
        debug.log("", debug.P_MID, "");
        print(_MID.names[mid.id] + " (client id %d, %s)" % (client_obj.id, "tcp" if sock_type == socket.SOCK_STREAM else "udp"), end='');
        if (mid.num_params >= 1 and params != None):
            print(": ", end='');
            i = 0;
            for param in params:
                print(("(%s): %" + mid.ft_params[i].printf_char) % (mid.ft_params[i].type_name, param), end='');
                i += 1;
                print("") if i >= len(params) else print(", ", end='');
        else:
            print("");
    else:
        debug.log("could not print params, required %d params, but %d params given" % (len(params), mid.num_params), debug.P_WARNING);