#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <functional>
#include <memory>

#include "../message/MID.h"

struct Socket;

namespace msg {

    enum ResponseCode {

        RESPONSE_NONE,
        RESPONSE_SUCCESS,
        RESPONSE_FAIL,
        RESPONSE_TIMEOUT
    };

    enum MsgCallbackType {

        CALLBACK_NONE,
        CALLBACK_UNIQUE_ID,
        CALLBACK_MID,
        CALLBACK_MID_LOOP,
        CALLBACK_MID_ANY
    };

    struct MsgCallback {

        MsgCallback(std::function<ResponseCode()>& f, CMID m, unsigned short i, MsgCallbackType t) : func(f), mid(m), id(i), type(t) { }

        std::function<ResponseCode()> func;
        unsigned short id;
        CMID mid;
        MsgCallbackType type;
        int num_callbacks_left = -1;
    };

    typedef std::shared_ptr<MsgCallback> MsgCallbackPtr;

    extern MsgCallbackPtr make_unique_id_callback (std::function<ResponseCode()> callback, 
                                                    CMID mid, unsigned short unique_id, Socket* add_to_sock = NULL);

    extern MsgCallbackPtr make_MID_callback       (std::function<ResponseCode()> callback, 
                                                    CMID mid, int num_callbacks = 1, Socket* add_to_sock = NULL);

    extern MsgCallbackPtr make_MID_once_callback  (std::function<ResponseCode()> callback, 
                                                    CMID mid, Socket* add_to_sock = NULL);

    extern MsgCallbackPtr make_MID_loop_callback  (std::function<ResponseCode()> callback, 
                                                    CMID mid, Socket* add_to_sock = NULL);

    extern MsgCallbackPtr make_any_MID_callback   (std::function<ResponseCode()> callback, 
                                                    int num_callbacks = 1, Socket* add_to_sock = NULL);

    extern unsigned int get_unique_callback_id(CMID mid);
};

#endif