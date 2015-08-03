import debug;
import _MID;

NONE = 0;
UNIQUE_ID = 1;
MID = 2;
MID_LOOP = 3;
MID_ANY = 4;
RESPONSE = 5;

RESPONSE_NONE = 0;
RESPONSE_SUCCESS = 1;
RESPONSE_FAIL = 2;
RESPONNSE_TIMEOUT = 3;

id_inc = 1;

class MessageCallback:

    func = None;
    id = 0;
    mid = _MID.UNKNOWN;
    type = NONE;
    num_callbacks_left = 0;

    def __init__(self, func, mid, id, type):
        self.func = func;
        self.mid = mid;
        self.id = id;
        self.type = type;
        pass;

def make_unique_id_callback(callback_func, mid, unique_id):
    cb = MessageCallback(callback_func, mid, unique_id, UNIQUE_ID);
    return cb;

def make_MID_callback(callback_func, mid, num_callbacks = 1):
    cb = MessageCallback(callback_func, mid, 0, MID);
    cb.num_callbacks_left = num_callbacks;
    return cb;

def make_MID_callback_once(callback_func, mid):
    cb = MessageCallback(callback_func, mid, 0, MID);
    cb.num_callbacks_left = 1;
    return cb;

def make_MID_callback_loop(callback_func, mid):
    cb = MessageCallback(callback_func, mid, 0, MID_LOOP);
    return cb;

def make_MID_any_callback(callback_func, num_callbacks = 1):
    cb = MessageCallback(callback_func, None, 0, MID_ANY);
    cb.num_callbacks_left = num_callbacks;
    return cb;

def make_response_callback(callback_func, unique_id = -1):
    if (unique_id): unique_id = id_inc;
    cb = MessageCallback(callback_func, _MID.RESPONSE, unique_id, UNIQUE_ID);
    return cb;

def get_unique_id():
    global id_inc;
    id_inc += 1;
    return id_inc;