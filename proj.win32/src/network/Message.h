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

		FormatType::FormatType() { }
		FormatType::FormatType(char c, short l);
		
		bool operator==(const FormatType& a) {
			return *this == a;
		}
		bool operator!=(const FormatType& a) {
			return *this != a;
		}
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
		FormatType* ft_params = NULL;
		int total_param_bytes = 0;
		int num_params = 0;

		MID::MID(int num_args, ...);
	};

	#define CMID const message::MID*

	extern int MID_id;
	extern std::vector<MID*> MID_list;
	
	extern CMID MID_UNKNOWN;
	extern CMID MID_CLIENT_ID;
	extern CMID MID_CLIENT_USER_PASS;

	//================== Message begin ==================

	extern char byte_buffer[1024];
	extern int byte_offset;
	extern std::vector<void*> param_list;

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
	extern CMID extract_mid(char* buffer, int buffer_len);
	extern void unpack(void* dest, char* buffer, int buffer_len);
	extern void extract_params(CMID mid, char* byte_data, int byte_data_len);
}

#endif