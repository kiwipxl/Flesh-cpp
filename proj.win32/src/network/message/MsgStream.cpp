#include "MsgStream.h"

#include "../sockets/Socket.h"
#include "Message.h"

using msg::MsgStream;

char msg::byte_buffer[1024];
int msg::byte_offset;
u_short callback_id_inc = 1;

MsgStream& MsgStream::operator<<(msg::Param* p) {
    check_MID_add();
    if (p != NULL) cpy_to_buf(p->data, p->len);
    return *this;
}

void MsgStream::write_callback_id() {
    cpy_to_buf(&callback_id_inc, sizeof(callback_id_inc));
    ++callback_id_inc;
}
