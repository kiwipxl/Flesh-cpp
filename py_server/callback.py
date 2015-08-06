import debug;
import _MID;
import time;

TIMEOUT_SHORT = 2.5;
TIMEOUT_NORMAL = 8000000.0;
TIMEOUT_LONG = 15.0;

class MessageCallback:

    func = None;
    id = 0;
    creation_time = 0;
    timeout_len = TIMEOUT_NORMAL;

    def __init__(self, mid, func):
        self.mid = mid;
        self.func = func;
        self.creation_time = time.time();
        self.timeout_len = TIMEOUT_NORMAL;

def make_MID_callback(mid, callback_func):
    cb = MessageCallback(mid, callback_func);
    return cb;