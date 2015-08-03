#include "MsgStream.h"

#include "Message.h"
#include "Callback.h"

using msg::MsgStream;

char msg::byte_buffer[1024];
int msg::byte_offset;

MsgStream& MsgStream::operator<<(msg::Param* p) {
    check_MID_add();
    if (p != NULL) cpy_to_buf(p->data, p->len);
    return *this;
}

void MsgStream::write_callback_id(CMID v) {
    cpy_to_buf(msg::get_unique_callback_id(v), sizeof(u_short));
}
