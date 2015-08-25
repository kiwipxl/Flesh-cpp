#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <ctime>

#include "network/Defines.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

const float TIMEOUT_NONE = 0.0f;
const float TIMEOUT_SHORT = 2.5f;
const float TIMEOUT_MODERATE = 5.0f;
const float TIMEOUT_LONG = 10.0f;

enum CallbackResult {

    CALLBACK_RESULT_UNKNOWN,
    CALLBACK_RESULT_SUCCESS,
    CALLBACK_RESULT_TIMEOUT
};

struct MIDCallback {

    MIDCallback(CMID _mid, CallbackFunc& _func, float _timeout_len, bool _remove_after_call) : 
        mid(_mid), func(_func), timeout_len(_timeout_len), remove_after_call(_remove_after_call) {
        reset_timeout();
    }

    CallbackFunc func;
    CMID mid;
    time_t creation_time;
    float timeout_len;
    bool remove_after_call;

    void reset_timeout() {
        creation_time = time(&creation_time);
    }
};

extern CallbackPtr make_MID_callback(CMID mid, CallbackFunc callback, float timeout_len = TIMEOUT_NONE, bool remove_after_call = false);
extern void process_message_callback(MessagePtr message);
extern void process_all_callbacks(sock::SocketPoll& poll);

END_MSG_NS
END_NETWORK_NS

#endif