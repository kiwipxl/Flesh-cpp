#ifndef _MSG_STREAM_H_
#define _MSG_STREAM_H_

#include <cassert>

#include "debug/Logger.h"
#include "network/message/MID.h"
#include "network/Defines.h"

BEGIN_NETWORK_NS

namespace msg {

    extern char byte_buffer[1024];
    extern int byte_offset;

    struct Param;
    class Stream {

        public:
            Stream() { byte_offset = 0; }

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

            Stream& operator<<(CMID v) {
                cf_assert(added_MID, sstream << "cannot add an MID to a stream when one has already been added");
                cpy_to_buf(&v->id, sizeof(int)); added_MID = true; header_complete = true; mid = v; return *this;
            }

            Stream& operator<<(MID_enum v) {
                cf_assert(added_MID, sstream << "cannot add an MID_enum to a stream when one has already been added");
                cpy_to_buf((int)v, sizeof(int)); added_MID = true; header_complete = true; mid = MID_list[v]; return *this;
            }

            Stream& operator<<(char* str) {
                check_MID_add();
                cpy_to_buf(str, strlen(str) + 1); return *this;
            }

            Stream& operator<<(std::string str) {
                check_MID_add();
                cpy_to_buf(str.c_str(), str.length() + 1); return *this;
            }

            Stream& operator<<(Param* p);

            Stream& operator>>(int i) { byte_offset -= i; return *this; }

            ~Stream() {
                cf_assert(!header_complete, sstream << "byte stream must be complete whenever used - MID required");
            }

        private:
            bool added_MID = false;
            bool header_complete = false;

            inline void check_MID_add() {
                cf_assert(!added_MID, sstream << "an MID must be added to the stream first, before any other values");
            }
    };
};

END_NETWORK_NS

#endif