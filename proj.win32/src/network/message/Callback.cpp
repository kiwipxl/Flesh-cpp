#include "Callback.h"

namespace msg {

    std::unique_ptr<MsgCallback> make_unique_id_callback(std::function<ResponseCode()> callback, CMID mid, unsigned int unique_id) {
        std::unique_ptr<MsgCallback> cb(new MsgCallback(callback, mid, unique_id, CALLBACK_UNIQUE_ID));
        return cb;
    }

    std::unique_ptr<MsgCallback> make_MID_callback(std::function<ResponseCode()> callback, CMID mid, int num_callbacks) {
        std::unique_ptr<MsgCallback> cb(new MsgCallback(callback, mid, 0, CALLBACK_MID));
        cb->num_callbacks_left = num_callbacks;
        return cb;
    }

    std::unique_ptr<MsgCallback> make_MID_once_callback(std::function<ResponseCode()> callback, CMID mid) {
        std::unique_ptr<MsgCallback> cb(new MsgCallback(callback, mid, mid->callback_id_inc, CALLBACK_MID));
        cb->num_callbacks_left = 1;
        return cb;
    }

    std::unique_ptr<MsgCallback> make_MID_loop_callback(std::function<ResponseCode()> callback, CMID mid) {
        std::unique_ptr<MsgCallback> cb(new MsgCallback(callback, mid, mid->callback_id_inc, CALLBACK_MID_LOOP));
        return cb;
    }

    std::unique_ptr<MsgCallback> make_MID_any_callback(std::function<ResponseCode()> callback, int num_callbacks) {
        std::unique_ptr<MsgCallback> cb(new MsgCallback(callback, NULL, num_callbacks, CALLBACK_MID_ANY));
        return cb;
    }
};
