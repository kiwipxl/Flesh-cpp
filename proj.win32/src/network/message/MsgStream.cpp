#include "MsgStream.h"

char msg::byte_buffer[1024];
int msg::byte_offset;
u_short callback_id_inc = 1;

inline void msg::ByteStream::write_callback_id() {
    cpy_to_buf(&callback_id_inc, sizeof(callback_id_inc));
    ++callback_id_inc;
}