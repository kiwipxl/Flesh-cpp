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
    
    const float TIMEOUT_NONE = 0.0f;
    const float TIMEOUT_SHORT = 2.5f;
    const float TIMEOUT_MODERATE = 5.0f;
    const float TIMEOUT_LONG = 10.0f;

    enum CallbackResult {

        CALLBACK_RESULT_SUCCESS, 
        CALLBACK_RESULT_TIMEOUT
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