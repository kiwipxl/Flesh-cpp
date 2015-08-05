#include "Callback.h"

#include "../sockets/Socket.h"
#include "Message.h"

namespace msg {
    
    CallbackPtr make_MID_callback(CallbackFunc callback, CMID mid) {
        return CallbackPtr(new MIDCallback(callback, mid));
    }
};
