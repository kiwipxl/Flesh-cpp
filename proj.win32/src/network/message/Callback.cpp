#include "Callback.h"

#include "../sockets/Socket.h"

namespace msg {

    MsgCallbackPtr make_unique_id_callback(std::function<ResponseCode()> callback, CMID mid, unsigned short unique_id, Socket* add_to_sock) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, unique_id, CALLBACK_UNIQUE_ID));
        if (add_to_sock) add_to_sock->add_callback(cb);
        return cb;
    }

    MsgCallbackPtr make_MID_callback(std::function<ResponseCode()> callback, CMID mid, int num_callbacks, Socket* add_to_sock) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, 0, CALLBACK_MID));
        cb->num_callbacks_left = num_callbacks;
        if (add_to_sock) add_to_sock->add_callback(cb);
        return cb;
    }

    MsgCallbackPtr make_MID_once_callback(std::function<ResponseCode()> callback, CMID mid, Socket* add_to_sock) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, mid->callback_id_inc, CALLBACK_MID));
        cb->num_callbacks_left = 1;
        if (add_to_sock) add_to_sock->add_callback(cb);
        return cb;
    }

    MsgCallbackPtr make_MID_loop_callback(std::function<ResponseCode()> callback, CMID mid, Socket* add_to_sock) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, mid->callback_id_inc, CALLBACK_MID_LOOP));
        if (add_to_sock) add_to_sock->add_callback(cb);
        return cb;
    }

    MsgCallbackPtr make_MID_any_callback(std::function<ResponseCode()> callback, int num_callbacks, Socket* add_to_sock) {
        MsgCallbackPtr cb(new MsgCallback(callback, NULL, num_callbacks, CALLBACK_MID_ANY));
        if (add_to_sock) add_to_sock->add_callback(cb);
        return cb;
    }

    unsigned int get_unique_callback_id(CMID mid) {
        return mid->callback_id_inc;
    }
};
