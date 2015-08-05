#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <functional>
#include <memory>
#include <ctime>

#include "../message/MID.h"

struct Socket;

namespace msg {

    typedef std::function<void()> CallbackFunc;

    struct MIDCallback {

        MIDCallback(CallbackFunc& f, CMID m) : func(f), mid(m) {
            creation_time = time(&creation_time);
        }

        CallbackFunc func;
        CMID mid;
        time_t creation_time;
        float timeout_len = 5000000.0f;
    };

    typedef std::shared_ptr<MIDCallback> MIDCallbackPtr;

    extern MIDCallbackPtr make_MID_callback(CallbackFunc callback, CMID mid);
};

#endif