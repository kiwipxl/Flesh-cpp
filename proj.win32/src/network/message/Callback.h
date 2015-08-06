#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <functional>
#include <memory>
#include <ctime>

#include "MID.h"

struct Socket;
namespace msg {
    struct Message;
}

namespace msg {

    enum DefaultTimeoutLength {

        TIMEOUT_NONE = 0, 
        TIMEOUT_SHORT = 2000,
        TIMEOUT_MODERATE = 5000,
        TIMEOUT_LONG = 10000
    };

    enum CallbackResult {

        CALLBACK_SUCCESS, 
        CALLBACK_TIMEOUT
    };

    typedef std::function<void(Message*)> CallbackFunc;

    struct MIDCallback {

        MIDCallback(CMID m, CallbackFunc& f, float t_len) : func(f), mid(m), timeout_len(t_len) {
            creation_time = time(&creation_time);
        }

        CallbackFunc func;
        CMID mid;
        time_t creation_time;
        float timeout_len;
    };

    typedef std::shared_ptr<MIDCallback> CallbackPtr;

    extern CallbackPtr make_MID_callback(CMID mid, CallbackFunc callback, float timeout_len = TIMEOUT_NONE);
};

#endif