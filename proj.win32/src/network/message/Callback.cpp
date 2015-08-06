#include "Callback.h"

#include "../sockets/Socket.h"
#include "Message.h"

namespace msg {

    CallbackPtr make_MID_callback(CMID mid, CallbackFunc callback, float timeout_len) {
        return CallbackPtr(new MIDCallback(mid, callback, timeout_len));
    }
};
