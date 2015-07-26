from __future__ import print_function;
import sys;
import struct;
import socket;
import debug;

class FormatType():

    struct_char = '';
    printf_char = '';
    len = 0;
    type_name = "";

    def __init__(self, schar, pchar, len, name):
        self.struct_char = schar;
        self.printf_char = pchar;
        self.len = len;
        self.type_name = name;

#format types for packing and unpacking byte data
FT_CHAR                     = FormatType('c', 'c', 1, "char");
FT_SIGNED_CHAR              = FormatType('b', 'c', 1, "schar");
FT_UNSIGNED_CHAR            = FormatType('B', 'c', 1, "uchar");
FT_BOOL                     = FormatType('?', 'd', 1, "bool");
FT_SHORT                    = FormatType('h', 'd', 2, "short");
FT_UNSIGNED_SHORT           = FormatType('H', 'd', 2, "ushort");
FT_INT                      = FormatType('i', 'i', 4, "int");
FT_UNSIGNED_INT             = FormatType('I', 'u', 4, "uint");
FT_LONG                     = FormatType('l', 'li', 8, "long");
FT_UNSIGNED_LONG            = FormatType('L', 'lu', 8, "ulong");
FT_LONG_LONG                = FormatType('q', 'lli', 8, "llong");
FT_UNSIGNED_LONG_LONG       = FormatType('Q', 'llu', 8, "ullong");
FT_FLOAT                    = FormatType('f', 'f', 4, "float");
FT_DOUBLE                   = FormatType('d', 'f', 8, "double");
FT_CHAR_ARRAY               = FormatType('s', 's', 1, "char*");
FT_VOID_POINTER             = FormatType('p', 'lu', 4, "void*");

MID_id = 0;
MID_list = [];

class MID():

    id = 0;
    ft_params = [];
    total_param_bytes = 0;
    num_params = 0;

    def __init__(self, *ft_params_list):
        global MID_id;
        global MID_list;

        self.id = MID_id;
        self.ft_params = ft_params_list;
        i = 0;
        for param in ft_params_list:
            self.total_param_bytes += param.len;
            i += 1;
        self.num_params = i;

        MID_id += 1;
        MID_list.append(self);

#MID syntax note:
#MID_SEND_XXX - a message to send to a client
#MID_RECV_XXX - a message to receive from a client
#MID_XXX - a message that can be both sent and received to and from clients

MID_UNKNOWN                                     = MID();

#sends a client id of a specified client
MID_SEND_CLIENT_ID                              = MID(FT_INT);
#receives a request to register a username and password from a client
MID_RECV_CLIENT_LOGIN_USER_PASS                 = MID(FT_CHAR_ARRAY, FT_CHAR_ARRAY);
#receives a request to register a username and password from a client
MID_RECV_CLIENT_REGISTER_USER_PASS              = MID(FT_CHAR_ARRAY, FT_CHAR_ARRAY);

#begins sending a relay test which ping pongs for infinite time
MID_BEGIN_RELAY_TEST                            = MID();
#ping pong relay test messages
MID_RELAY_TEST                                  = MID(FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT, FT_UNSIGNED_SHORT);
#init ping pong udp message to test udp communication
MID_UDP_INIT_PING_PONG                          = MID();
#ping pong udp messages
MID_UDP_PING_PONG                               = MID();

MID_RECV_UDP_SERVER_COMMUNICATION_SUCCESS       = MID();
#requests a client to bind to a random port to allow peers to send messages to
MID_SEND_UDP_PEER_BIND_REQUEST                  = MID(FT_INT, FT_CHAR_ARRAY);
#receives the binded udp port from a client after the request was made
MID_RECV_UDP_PEER_BIND_PORT_SUCCESS             = MID(FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT);
#the request to bind a udp port on a client failed
MID_RECV_UDP_PEER_BIND_PORT_FAILED              = MID(FT_INT, FT_CHAR_ARRAY);
#received when a client successfully connect to their peer
MID_RECV_PEER_CONNECT_SUCCESS                   = MID();

#sent once a peer has joined and successfully communicated with the client
MID_SEND_PEER_JOIN                              = MID(FT_CHAR_ARRAY, FT_UNSIGNED_SHORT);
#sent once a client has left the server (tcp, udp connection fail, ect) and needs to tell the client about it
MID_SEND_PEER_LEAVE                             = MID(FT_CHAR_ARRAY, FT_UNSIGNED_SHORT);
#received if a client is unable to communicate with other peers
MID_RECV_PEER_LEAVE                             = MID(FT_CHAR_ARRAY, FT_UNSIGNED_SHORT);

#server sends server udp binded port to client
MID_SEND_SERVER_BINDED_UDP_PORT                 = MID(FT_UNSIGNED_SHORT);
#client sends client binded udp port to server
MID_RECV_CLIENT_BINDED_UDP_PORT                 = MID(FT_UNSIGNED_SHORT);

#put all MID_x variables into a name array so messages can be debugged easier
MID_names = MID_id * [None];
for k, v in list(locals().iteritems()):
    if (k != None and len(k) >= 4 and k[0:4] == "MID_" and isinstance(v, MID)):
        MID_names[v.id] = k;

byte_buffer = bytearray(1024);
byte_offset = 0;

def pack_message(mid, params = None):
    byte_offset = 4;
    i = 0;
    byte_buffer[0:4] = struct.pack(FT_INT.struct_char, mid.id);
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
    mid = MID_UNKNOWN;
    if (len(byte_data) >= 4):
        id = struct.unpack("i", byte_data[0:4])[0];
        if (id >= 0 and id < len(MID_list)):
            mid = MID_list[id];
    return mid;

import time;

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
                  (MID_names[mid.id], len(byte_data), mid.total_param_bytes + 4), debug.P_WARNING);
        return (params, -1);

def send(sock, client, mid, params = None):
    if (sock.type == socket.SOCK_STREAM):
        sock.send(pack_message(mid, params));
    else:
        if (client.c_tcp_port <= 0): debug.log("c_tcp_port is less than zero when trying to send tcp message", debug.P_WARNING); return;
        sock.sendto(pack_message(mid, params), (client.ip, client.c_tcp_port));

def send_tcp(tcp_sock, mid, params = None):
    tcp_sock.send(pack_message(mid, params));

def send_udp(udp_sock, ip, port, mid, params = None):
    if (port <= 0): debug.log("port is less than zero when trying to send_udp message", debug.P_WARNING); return;
    udp_sock.sendto(pack_message(mid, params), (ip, port));

def broadcast(sock_list, mid, params = None):
    for sock in sock_list:
        sock.send(pack_message(mid, params));

def print_params(client_obj, sock_type, mid, params):
    if (mid.num_params >= 1 and mid.num_params == len(params)):
        debug.log("", debug.P_MID, "");
        print(MID_names[mid.id] + " (client id %d, %s): " % (client_obj.id, "tcp" if sock_type == socket.SOCK_STREAM else "udp"), end='');
        i = 0;
        for param in params:
            print(("(%s): %" + mid.ft_params[i].printf_char) % (mid.ft_params[i].type_name, param), end='');
            i += 1;
            print("") if i >= len(params) else print(", ", end='');
    else:
        debug.log("could not print params, required %d params, but %d params given" % (len(params), mid.num_params), debug.P_WARNING);