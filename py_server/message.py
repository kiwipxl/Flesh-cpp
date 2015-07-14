import sys;
import struct;

class MID():

    id = 0;
    def __init__(self, *param_types):
        print(param_types);

#format types for packing and unpacking byte data
FT_CHAR = 'c';
FT_SIGNED_CHAR = 'b';
FT_UNSIGNED_CHAR = 'B';
FT_BOOL = '?';
FT_SHORT = 'h';
FT_UNSIGNED_SHORT = 'H';
FT_INT = 'i';
FT_UNSIGNED_INT = 'I';
FT_LONG = 'l';
FT_UNSIGNED_LONG = 'L';
FT_LONG_LONG = 'q';
FT_UNSIGNED_LONG_LONG = 'Q';
FT_FLOAT = 'f';
FT_DOUBLE = 'd';
FT_CHAR_ARRAY = 's';
FT_VOID_POINTER = 'p';

MID_UNKNOWN = MID(None);
MID_UNKNOWN = MID(None);
MID_UNKNOWN = MID(None);

byte_buffer = bytearray(1024);
byte_offset = 0;

def encode(msg_id, *params):
    byte_offset = 0;
    for param in params:
        t = type(param).__name__[0];
        print(str(t));
        if (t == 's'):
            s = param;
        else:
            s = struct.pack(t, param);
        byte_buffer[byte_offset:byte_offset + s.__len__()] = s;
        byte_offset += s.__len__();
    print(str(byte_buffer) + ", len: %s" % byte_offset);
    return byte_buffer;

def send(sock, msg_id, *params):
    sock.send(encode(msg_id, *params));

def broadcast_raw(sock_list, msg_id, *params):
    for sock in sock_list:
        sock.send(buffer(encode(msg_id, params).encode()));