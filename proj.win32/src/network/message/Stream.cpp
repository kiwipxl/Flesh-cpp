#include "network/message/Stream.h"

#include "network/message/Callback.h"
#include "network/message/Message.h"

BEGIN_NETWORK_NS

namespace msg {

    char byte_buffer[1024];
    int byte_offset;

    Stream& Stream::operator<<(Param* p) {
        check_MID_add();
        if (p != NULL) cpy_to_buf(p->data, p->len);
        return *this;
    }
};

END_NETWORK_NS