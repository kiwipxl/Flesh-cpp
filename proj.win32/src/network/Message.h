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

		const char* printf_format;
		const short len;

		FormatType(const char* c, const short l);
	};

	#define CFTYPE const message::FormatType*

	//format types for packing and unpacking byte data
	extern CFTYPE FT_CHAR;
	extern CFTYPE FT_SIGNED_CHAR;
	extern CFTYPE FT_UNSIGNED_CHAR;
	extern CFTYPE FT_BOOL;
	extern CFTYPE FT_SHORT;
	extern CFTYPE FT_UNSIGNED_SHORT;
	extern CFTYPE FT_INT;
	extern CFTYPE FT_UNSIGNED_INT;
	extern CFTYPE FT_LONG;
	extern CFTYPE FT_UNSIGNED_LONG;
	extern CFTYPE FT_LONG_LONG;
	extern CFTYPE FT_UNSIGNED_LONG_LONG;
	extern CFTYPE FT_FLOAT;
	extern CFTYPE FT_DOUBLE;
	extern CFTYPE FT_CHAR_ARRAY;
	extern CFTYPE FT_VOID_POINTER;

	//================== MID begin ==================

	struct MID {

		const int id = 0;
		CFTYPE* ft_params = NULL;
		int total_param_bytes = 0;
		int num_params = 0;

		MID(int num_args, ...);
	};

	#define CMID const message::MID*

	extern int MID_id;
	extern std::vector<MID*> MID_list;
	
	extern CMID MID_UNKNOWN;
	extern CMID MID_CLIENT_ID;
	extern CMID MID_CLIENT_USER_PASS;
	extern CMID MID_RELAY_TEST;

	//================== Parameters begin ==================

	struct Param {

		char* data;
		int len;
	};

	extern const int MAX_NUM_PARAMS;
	extern Param* param_list[];
	extern int param_list_size;
	extern int param_tbytes;
	extern CMID last_extracted_mid;

	//================== Message begin ==================

	extern char byte_buffer[1024];
	extern int byte_offset;

	class ByteStream {

		public:
			ByteStream() { byte_offset = 0; }

			template <class T> inline void cpy_to_buf(const T* v, int len) {
				memcpy(byte_buffer + byte_offset, v, len);
				byte_offset += len;
			}

			template <class T> ByteStream& operator<<(const T& v) { cpy_to_buf(&v, sizeof(v)); return *this; }
			ByteStream& operator<<(CMID v) { cpy_to_buf(&v->id, sizeof(int)); return *this; }
			ByteStream& operator<<(char* str) { cpy_to_buf(str, 8); return *this; }
	};

	extern void send(Socket* sock, ByteStream& stream);
	extern CMID extract_mid(char* buffer, int buffer_len);
	extern void extract_params(CMID mid, char* byte_data, int byte_data_len);
	extern void clear_param_list();
	extern void print_extracted_params();
}

#endif