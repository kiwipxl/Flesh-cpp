#ifndef _MSG_STREAM_H_
#define _MSG_STREAM_H_

#include "Message.h"

namespace msg {

    extern char byte_buffer[1024];
    extern int byte_offset;

    #define MSG_HEADER_SIZE 8

    class ByteStream {

    public:
        ByteStream() { byte_offset = 0; }

        template <class T> inline void cpy_to_buf(const T* v, int len) {
            memcpy(byte_buffer + byte_offset, v, len);
            byte_offset += len;
        }

        template <class T> ByteStream& operator<<(const T& v) { check_MID_add(); cpy_to_buf(&v, sizeof(v)); return *this; }
        ByteStream& operator<<(CMID v) { cpy_to_buf(&v->id, sizeof(int)); added_MID = true; write_callback_id(); return *this; }
        ByteStream& operator<<(char* str) { check_MID_add(); cpy_to_buf(str, strlen(str) + 1); return *this; }
        ByteStream& operator<<(Param* p) { check_MID_add(); if (p != NULL) cpy_to_buf(p->data, p->len); return *this; }

        ~ByteStream() {
            if (!header_complete) assert("byte stream must be complete whenever used - MID required");
        }

    private:
        bool added_MID = false;
        bool header_complete = false;

        inline void write_callback_id();

        inline void check_MID_add() {
            if (!added_MID) assert("an MID must be added to the stream first, before any other values");
        }
    };
};

#endif