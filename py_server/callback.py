import debug;
import _MID;
import time;
import msg;

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

def process_message(message):
    message.callback_result = CALLBACK_RESULT_SUCCESS;
    n = 0;
    for i in range(0, len(message.client_obj.callbacks)):
        cb = message.client_obj.callbacks[n];
        if (cb.mid == message.mid or cb.mid == _MID.ALL):
            cb.func(message);
            if (cb.remove_after_call):
                del message.client_obj.callbacks[n];
                n -= 1;
            else:
                cb.reset_timeout();
        n += 1;

def process_callbacks(client_obj):
    n = 0;
    for i in range(0, len(client_obj.callbacks)):
        cb = client_obj.callbacks[n];
        if (cb.timeout_len != TIMEOUT_NONE and (time.time() - cb.creation_time) >= cb.timeout_len):
            debug.log("callback timeout for %s" % cb.mid.name, debug.P_INFO);
            m = msg.Message();
            m.mid = _MID.UNKNOWN;
            m.client_obj = client_obj;
            m.callback_result = CALLBACK_RESULT_TIMEOUT;
            client_obj.callbacks[n].func(m);
            if (client_obj and not client_obj.left):
                if (cb.remove_after_call):
                    del client_obj.callbacks[n];
                    n -= 1;
                else:
                    cb.reset_timeout();
            else:
                return;
        n += 1;