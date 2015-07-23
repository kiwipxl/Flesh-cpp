#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <stdarg.h>
#include <sstream>
#include <vector>
#include "sockets/Socket.h"

namespace msg {

	//================== Format types begin ==================

	struct FormatType {

		const char* printf_format;
		const short len;
		const char* type_name;

		FormatType(const char* c, const short l, const char* name);
	};

	#define CFTYPE const msg::FormatType*

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

	#define CMID const msg::MID*

	extern int MID_id;
	extern std::vector<CMID> MID_list;
	extern std::vector<std::string> MID_names;

	extern CMID MID_UNKNOWN;
	extern CMID MID_CLIENT_ID;
	extern CMID MID_CLIENT_USER_PASS;
	extern CMID MID_BEGIN_RELAY_TEST;
	extern CMID MID_RELAY_TEST;
    extern CMID MID_GET_TCP_AND_UDP_CLIENT_PORTS;
    extern CMID MID_UDP_PING_PONG;
    extern CMID MID_GAME_PEER_JOIN;
    extern CMID MID_GAME_PEER_LEAVE;

	//================== Hack to get message variable names ==================

	#define PP_CAT(a, b) PP_CAT_I(a, b)
	#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
	#define PP_CAT_II(p, res) res

	#define UNIQUE_NAME(base) PP_CAT(base, __COUNTER__)

	struct MID_AutoName {

		MID_AutoName(std::string name) {
			int index = name.find("MID_");
			int index_end;
			if (index != -1) index_end = name.find("=");
				if (index_end != -1) MID_names.push_back(name.substr(index, index_end - index).c_str() + '\0');
		}
	};

	#define ADD_MID_NAME(name) name; msg::MID_AutoName UNIQUE_NAME(_unique_) = msg::MID_AutoName(#name);

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
	extern const int MAX_PRINT_BUF;
	extern char print_buf[];

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
			ByteStream& operator<<(char* str) { cpy_to_buf(str, strlen(str) + 1); return *this; }
			ByteStream& operator<<(Param* p) { if (p != NULL) cpy_to_buf(p->data, p->len); return *this; }
	};

	void init();
	void send(Socket& sock, ByteStream& stream);
	CMID extract_mid(char* buffer, int buffer_len);
	void extract_params(CMID mid, char* byte_data, int byte_data_len);
	void clear_param_list();
	void print_extracted_params();
    inline const char* get_MID_name(CMID mid);
}

#endif