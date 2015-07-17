#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <stdarg.h>
#include <sstream>
#include <vector>
#include "ReadableType.h"
#include "Socket.h"

namespace message {

	//================== Format types begin ==================

	struct FormatType {

		char chr;
		short len;

		FormatType::FormatType(char c, short l) : chr(c), len(l) { }
	};

	//format types for packing and unpacking byte data
	extern const FormatType FT_CHAR;
	extern const FormatType FT_SIGNED_CHAR;
	extern const FormatType FT_UNSIGNED_CHAR;
	extern const FormatType FT_BOOL;
	extern const FormatType FT_SHORT;
	extern const FormatType FT_UNSIGNED_SHORT;
	extern const FormatType FT_INT;
	extern const FormatType FT_UNSIGNED_INT;
	extern const FormatType FT_LONG;
	extern const FormatType FT_UNSIGNED_LONG;
	extern const FormatType FT_LONG_LONG;
	extern const FormatType FT_UNSIGNED_LONG_LONG;
	extern const FormatType FT_FLOAT;
	extern const FormatType FT_DOUBLE;
	extern const FormatType FT_CHAR_ARRAY;
	extern const FormatType FT_VOID_POINTER;

	//================== MID begin ==================

	struct MID {

		int id = 0;
		std::vector<FormatType> ft_params;
		int min_param_len = 0;

		MID::MID(...);
	};
	
	extern int MID_id;
	extern std::vector<MID*> MID_list;
	
	extern const MID MID_UNKNOWN;
	extern const MID MID_CLIENT_ID;
	extern const MID MID_CLIENT_USER_PASS;

	//================== Message begin ==================

	class Message {

		public:
			MID msg_id = MID_UNKNOWN;
			char* params;
			std::string raw_data;
	};

	extern char byte_buffer[1024];
	extern int byte_offset;

	class ByteStream {

		public:
			ByteStream() { byte_offset = 0; }

			template <class T> inline void cpy_to_buf(const T& v) {
				memcpy(byte_buffer + byte_offset, &v, sizeof(v));
				byte_offset += sizeof(v);
			}

			template <class T> ByteStream& operator<<(const T& v) { cpy_to_buf(v); return *this; }

			ByteStream& operator<<(const MID& v) { cpy_to_buf(v.id); return *this; }
	};

	extern void send(Socket* sock, ByteStream& stream);
}

#endif