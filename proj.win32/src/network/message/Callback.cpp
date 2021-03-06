#include "network/message/Callback.h"

#include "utility/Logger.h"
#include "network/message/MID.h"
#include "network/message/Message.h"
#include "network/sockets/Socket.h"
#include "network/sockets/SocketPoll.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

//private

//public
CallbackPtr make_MID_callback(CMID mid, CallbackFunc callback, float timeout_len, bool remove_after_call) {
    return CallbackPtr(new MIDCallback(mid, callback, timeout_len, remove_after_call));
}

void process_message_callback(MessagePtr message) {
    message->callback_result = msg::CALLBACK_RESULT_SUCCESS;
    for (int n = 0; n < message->sock->callbacks.size(); ++n) {
        if (message->sock->callbacks[n]->mid == message->mid) {
            message->sock->callbacks[n]->func(message.get());
            if (message->sock->callbacks[n]->remove_after_call) {
                message->sock->callbacks.erase(message->sock->callbacks.begin() + n);
                --n;
            }else {
                message->sock->callbacks[n]->reset_timeout();
            }
        }
    }
}

void process_all_callbacks(sock::SocketPoll& poll) {
    time_t t;
    for (int i = 0; i < poll.get_size(); ++i) {
        sock::Socket* sock = poll.get_sock_at(i);
        if (!sock) continue;
        for (int n = 0; n < sock->callbacks.size(); ++n) {
            msg::CallbackPtr& cb = sock->callbacks[n];
            if (cb->timeout_len != msg::TIMEOUT_NONE && (time(&t) - cb->creation_time) >= cb->timeout_len) {
                log_info << "callback timeout for " << cb->mid->name;
                msg::Message m;
                m.callback_result = msg::CALLBACK_RESULT_TIMEOUT;
                m.sock = sock;
                cb->func(&m);
                if (cb->remove_after_call) {
                    sock->callbacks.erase(sock->callbacks.begin() + n);
                    --n;
                }else {
                    cb->reset_timeout();
                }
            }
        }
    }
}

END_MSG_NS
END_NETWORK_NS