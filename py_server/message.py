import sys;
import struct;

class MID():

    id = 0;
    ft_params = [];
    min_param_len = 0;

    def __init__(self, *ft_params_list):
        global MID_id;
        global MID_list;

        self.id = MID_id;
        self.ft_params = ft_params_list;
        for param in ft_params_list:
            self.min_param_len += param.len;

        MID_id += 1;
        MID_list.append(self);

MID_id = 0;
MID_list = [];

class FormatType():

    char = '';
    len = 0;

    def __init__(self, char, len):
        self.char = char;
        self.len = len;

#format types for packing and unpacking byte data
FT_CHAR                     = FormatType('c', 1);
FT_SIGNED_CHAR              = FormatType('b', 1);
FT_UNSIGNED_CHAR            = FormatType('B', 1);
FT_BOOL                     = FormatType('?', 1);
FT_SHORT                    = FormatType('h', 2);
FT_UNSIGNED_SHORT           = FormatType('H', 2);
FT_INT                      = FormatType('i', 4);
FT_UNSIGNED_INT             = FormatType('I', 4);
FT_LONG                     = FormatType('l', 8);
FT_UNSIGNED_LONG            = FormatType('L', 8);
FT_LONG_LONG                = FormatType('q', 8);
FT_UNSIGNED_LONG_LONG       = FormatType('Q', 8);
FT_FLOAT                    = FormatType('f', 4);
FT_DOUBLE                   = FormatType('d', 8);
FT_CHAR_ARRAY               = FormatType('s', 0);
FT_VOID_POINTER             = FormatType('p', 4);

MID_UNKNOWN                         = MID();
MID_CLIENT_ID                       = MID(FT_CHAR_ARRAY, FT_CHAR_ARRAY, FT_INT, FT_BOOL);
MID_CLIENT_USER_PASS                = MID(FT_CHAR_ARRAY, FT_CHAR_ARRAY);

#put all MID_x variables into a name array so messages can be debugged easier
MID_names = MID_id * [None];
for k, v in list(locals().iteritems()):
    if (k != None and len(k) >= 4 and k[0:4] == "MID_" and isinstance(v, MID)):
        MID_names[v.id] = k;

byte_buffer = bytearray(1024);
byte_offset = 0;

def encode(mid, *params):
    byte_offset = 0;
    i = 0;
    for param in params:
        t = mid.ft_params[i];
        if (t.char == 's'):
            s = param;
        else:
            s = struct.pack(t.char, param);
        byte_buffer[byte_offset:byte_offset + s.__len__()] = s;
        byte_offset += s.__len__();
        i += 1;
    return byte_buffer;

def extract_mid(byte_data):
    mid = MID_UNKNOWN;
    if (len(byte_data) >= 4):
        id = struct.unpack("i", byte_data[0:4])[0];
        if (id >= 0 and id < len(MID_list)):
            mid = MID_list[id];
    return mid;

def extract_params(mid, byte_data):
    params = [];
    if (len(byte_data) - 4 >= mid.min_param_len):
        byte_offset = 4;
        for n in range(0, len(mid.ft_params)):
            t = mid.ft_params[n];
            if (t.char == 's'):
                s = "";
                l = 0;
                for c in byte_data[byte_offset:]:
                    l += 1;
                    if (c == '\0'): break;
                    s += c;
                byte_offset += l;
            else:
                s = struct.unpack(t.char, byte_data[byte_offset:byte_offset + t.len])[0];
                byte_offset += t.len;
            params.append(s);
    else:
        print("recv message %s is only %i bytes long when minimum is %i bytes" % (MID_names[mid.id], len(byte_data) - 4, mid.min_param_len));

    return params;

def send(sock, mid, *params):
    sock.send(encode(mid, *params));

def broadcast(sock_list, mid, *params):
    for sock in sock_list:
        sock.send(buffer(encode(mid, params).encode()));