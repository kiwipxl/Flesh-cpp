#include "network/message/Stream.h"

#include "network/message/Callback.h"
#include "network/message/Message.h"

using msg::Stream;

char msg::byte_buffer[1024];
int msg::byte_offset;

Stream& Stream::operator<<(msg::Param* p) {
    check_MID_add();
    if (p != NULL) cpy_to_buf(p->data, p->len);
    return *this;
}