#ifndef _MSG_STREAM_H_
#define _MSG_STREAM_H_

#include <cassert>

#include "MID.h"

namespace msg {

    extern char byte_buffer[1024];
    extern int byte_offset;

    #define MSG_HEADER_SIZE 8

    struct Param;
    class MsgStream {

        public:
            MsgStream() { byte_offset = 0; }

            CMID mid;

            template <class T> inline void cpy_to_buf(const T* v, int len) {
                memcpy(byte_buffer + byte_offset, v, len);
                byte_offset += len;
            }

            template <class T> MsgStream& operator<<(const T& v) { check_MID_add(); cpy_to_buf(&v, sizeof(v)); return *this; }
            MsgStream& operator<<(CMID v) {
                if (added_MID) assert("cannot add an MID to a MsgStream when one has already been added");
                cpy_to_buf(&v->id, sizeof(int)); added_MID = true; mid = v; write_callback_id(v); return *this;
            }
            MsgStream& operator<<(char* str) { check_MID_add(); cpy_to_buf(str, strlen(str) + 1); return *this; }
            MsgStream& operator<<(Param* p);

            ~MsgStream() {
                if (!header_complete) assert("byte stream must be complete whenever used - MID required");
            }

        private:
            bool added_MID = false;
            bool header_complete = false;

            void write_callback_id(CMID v);

            inline void check_MID_add() {
                if (!added_MID) assert("an MID must be added to the stream first, before any other values");
            }
    };
};

#endif