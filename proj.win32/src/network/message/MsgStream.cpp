#include "MsgStream.h"

#include "Message.h"

using msg::MsgStream;

char msg::byte_buffer[1024];
int msg::byte_offset;

MsgStream& MsgStream::operator<<(msg::Param* p) {
    check_MID_add();
    if (p != NULL) cpy_to_buf(p->data, p->len);
    return *this;
}

void MsgStream::write_callback_id(CMID v) {
    ++v->callback_id_inc;
    cpy_to_buf(&v->callback_id_inc, sizeof(u_short));
}
