#include "network/message/Stream.h"

#include "network/message/Callback.h"
#include "network/message/Message.h"
#include "network/message/MID.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

//public
char byte_buffer[1024];
int byte_offset;

Stream& Stream::operator<<(Param* p) {
    check_MID_add();
    if (p != NULL) cpy_to_buf(p->data, p->len);
    return *this;
}

Stream& Stream::operator<<(CMID v) {
    cf_assert(added_MID, sstream_cstr("cannot add an MID to a stream when one has already been added"));
    cpy_to_buf(&v->id, sizeof(int)); added_MID = true; header_complete = true; mid = v; return *this;
}

Stream& Stream::operator<<(MID_enum v) {
    cf_assert(added_MID, sstream_cstr("cannot add an MID_enum to a stream when one has already been added"));
    cpy_to_buf((int)v, sizeof(int)); added_MID = true; header_complete = true; mid = get_MID(v); return *this;
}

Stream& Stream::operator<<(char* str) {
    check_MID_add();
    cpy_to_buf(str, strlen(str) + 1); return *this;
}

Stream& Stream::operator<<(std::string str) {
    check_MID_add();
    cpy_to_buf(str.c_str(), str.length() + 1); return *this;
}

Stream& Stream::operator>>(int i) { byte_offset -= i; return *this; }

END_MSG_NS
END_NETWORK_NS