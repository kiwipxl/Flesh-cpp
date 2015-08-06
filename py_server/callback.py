import debug;
import _MID;
import time;

TIMEOUT_NONE = 0.0;
TIMEOUT_SHORT = 2.0;
TIMEOUT_NORMAL = 5.0;
TIMEOUT_LONG = 10.0;

CALLBACK_RESULT_UNKNOWN = 0;
CALLBACK_RESULT_SUCCESS = 1;
CALLBACK_RESULT_TIMEOUT = 2;

class MessageCallback:

    mid = None;
    func = None;
    creation_time = 0;
    timeout_len = TIMEOUT_NONE;
    remove_after_call = False;

    def __init__(self, mid, func, timeout_len = TIMEOUT_NONE, remove_after_call = False):
        self.mid = mid;
        self.func = func;
        self.timeout_len = timeout_len;
        self.remove_after_call = remove_after_call;
        self.reset_timeout();

    def reset_timeout(self):
        self.creation_time = time.time();

def make_MID_callback(mid, callback_func, timeout_len = TIMEOUT_NONE, remove_after_call = False):
    cb = MessageCallback(mid, callback_func, timeout_len, remove_after_call);
    return cb;