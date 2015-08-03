#include "Callback.h"

#include "../sockets/Socket.h"

namespace msg {

    MsgCallbackPtr make_unique_id_callback(std::function<ResponseCode()> callback, CMID mid, unsigned short unique_id) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, unique_id, CALLBACK_UNIQUE_ID));
        return cb;
    }

    MsgCallbackPtr make_MID_callback(std::function<ResponseCode()> callback, CMID mid, int num_callbacks) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, 0, CALLBACK_MID));
        cb->num_callbacks_left = num_callbacks;
        return cb;
    }

    MsgCallbackPtr make_MID_once_callback(std::function<ResponseCode()> callback, CMID mid) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, mid->callback_id_inc, CALLBACK_MID));
        cb->num_callbacks_left = 1;
        return cb;
    }

    MsgCallbackPtr make_MID_loop_callback(std::function<ResponseCode()> callback, CMID mid) {
        MsgCallbackPtr cb(new MsgCallback(callback, mid, mid->callback_id_inc, CALLBACK_MID_LOOP));
        return cb;
    }

    MsgCallbackPtr make_MID_any_callback(std::function<ResponseCode()> callback, int num_callbacks) {
        MsgCallbackPtr cb(new MsgCallback(callback, NULL, num_callbacks, CALLBACK_MID_ANY));
        return cb;
    }

    MsgCallbackPtr make_response_callback(std::function<ResponseCode()> callback, unsigned short unique_id) {
        MsgCallbackPtr cb(new MsgCallback(callback, _MID->RESPONSE, unique_id, CALLBACK_UNIQUE_ID));
        return cb;
    }

    unsigned int get_unique_callback_id(CMID mid) {
        return mid->callback_id_inc;
    }
};
