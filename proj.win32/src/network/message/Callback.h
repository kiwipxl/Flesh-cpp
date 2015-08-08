#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <ctime>
#include <functional>
#include <memory>

#include "network/message/MID.h"
#include "network/Defines.h"

BEGIN_NETWORK_NS

namespace sock {

    struct Socket;
    struct SocketPoll;
};

namespace msg {

    struct Message;
    typedef std::shared_ptr<Message> MessagePtr;

    const float TIMEOUT_NONE = 0.0f;
    const float TIMEOUT_SHORT = 2.5f;
    const float TIMEOUT_MODERATE = 5.0f;
    const float TIMEOUT_LONG = 10.0f;

    enum CallbackResult {

        CALLBACK_RESULT_UNKNOWN, 
        CALLBACK_RESULT_SUCCESS, 
        CALLBACK_RESULT_TIMEOUT
    };

    typedef std::function<void(Message*)> CallbackFunc;

    struct MIDCallback {

        MIDCallback(CMID m, CallbackFunc& f, float t_len, bool r_after_call) : func(f), mid(m), timeout_len(t_len), remove_after_call(r_after_call) {
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

    typedef std::shared_ptr<MIDCallback> CallbackPtr;

    extern CallbackPtr make_MID_callback(CMID mid, CallbackFunc callback, float timeout_len = TIMEOUT_NONE, bool remove_after_call = false);
    extern void process_message_callback(MessagePtr message);
    extern void process_all_callbacks(sock::SocketPoll& poll);
};

END_NETWORK_NS

#endif