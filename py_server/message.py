import sys;
import struct;

class MID():

    id = 0;
    ft_params = [];

    def __init__(self, *ft_params_list):
        global MID_id;
        global MID_list;

        self.id = MID_id;
        self.ft_params = ft_params_list;

        MID_id += 1;
        MID_list.append(self);

MID_id = 0;
MID_list = [];

#format types for packing and unpacking byte data
FT_CHAR                     = 'c';
FT_SIGNED_CHAR              = 'b';
FT_UNSIGNED_CHAR            = 'B';
FT_BOOL                     = '?';
FT_SHORT                    = 'h';
FT_UNSIGNED_SHORT           = 'H';
FT_INT                      = 'i';
FT_UNSIGNED_INT             = 'I';
FT_LONG                     = 'l';
FT_UNSIGNED_LONG            = 'L';
FT_LONG_LONG                = 'q';
FT_UNSIGNED_LONG_LONG       = 'Q';
FT_FLOAT                    = 'f';
FT_DOUBLE                   = 'd';
FT_CHAR_ARRAY               = 's';
FT_VOID_POINTER             = 'p';

MID_UNKNOWN                         = MID();
MID_CLIENT_ID                       = MID(FT_INT);
MID_CLIENT_USER_PASS                = MID(FT_CHAR_ARRAY, FT_CHAR_ARRAY);

byte_buffer = bytearray(1024);
byte_offset = 0;

def encode(mid, *params):
    byte_offset = 0;
    i = 0;
    for param in params:
        t = mid.ft_params[i];
        if (t == 's'):
            s = param;
        else:
            s = struct.pack(t, param);
        byte_buffer[byte_offset:byte_offset + s.__len__()] = s;
        byte_offset += s.__len__();
        i += 1;
    return byte_buffer;

def get_mid(byte_data):
    return len(byte_data) >= 4 if message.MID_list[struct.unpack("i", byte_data[0:4])[0]] else MID_UNKNOWN;

def send(sock, mid, *params):
    sock.send(encode(mid, *params));

def broadcast_raw(sock_list, mid, *params):
    for sock in sock_list:
        sock.send(buffer(encode(mid, params).encode()));