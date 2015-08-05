#include "Callback.h"

#include "../sockets/Socket.h"
#include "Message.h"

namespace msg {
    
    MIDCallbackPtr make_MID_callback(CallbackFunc callback, CMID mid) {
        MIDCallback cb(new MIDCallback(callback, mid));
        cb->num_callbacks_left = num_callbacks;
        return cb;
    }
};
