#ifndef _MSG_STREAM_H_
#define _MSG_STREAM_H_

#include "debug/Logger.h"
#include "network/Defines.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

extern char byte_buffer[1024];
extern int byte_offset;

class Stream {

    public:
        Stream() { byte_offset = 0; }
        ~Stream() {
            cf_assert(!header_complete, sstream_cstr("byte stream must be complete whenever used - MID required"));
        }

        CMID mid;

        template <class T> inline void cpy_to_buf(const T& v, int len) {
            memcpy(byte_buffer + byte_offset, &v, len);
            byte_offset += len;
        }

        template <class T> inline void cpy_to_buf(const T* v, int len) {
            memcpy(byte_buffer + byte_offset, v, len);
            byte_offset += len;
        }

        template <class T> Stream& operator<<(const T& v) {
            check_MID_add();
            cpy_to_buf(&v, sizeof(v)); return *this;
        }

        Stream& operator<<(CMID v);
        Stream& operator<<(MID_enum v);
        Stream& operator<<(char* str);
        Stream& operator<<(std::string str);
        Stream& operator<<(Param* p);
        
        Stream& operator>>(int i);

    private:
        bool added_MID = false;
        bool header_complete = false;

        inline void check_MID_add() {
            cf_assert(!added_MID, sstream_cstr("an MID must be added to the stream first, before any other values"));
        }
};

END_MSG_NS
END_NETWORK_NS

#endif