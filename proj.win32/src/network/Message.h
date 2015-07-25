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
		const CFTYPE* ft_params = NULL;
		int total_param_bytes = 0;
        int num_params = 0;
        const char* name;

		MID(int num_args, ...);
	};

	#define CMID msg::MID*

	extern int MID_id;
	extern std::vector<CMID> MID_list;
    extern std::vector<std::string> MID_names;

    //================== Hack to get message variable names ==================

    #define PP_CAT(a, b) PP_CAT_I(a, b)
    #define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
    #define PP_CAT_II(p, res) res

    #define UNIQUE_NAME(base) PP_CAT(base, __COUNTER__)

    struct MID_AutoName {

        MID_AutoName(std::string name) {
            int index = name.find(" ");
            int index_end;
            if (index != -1) index_end = name.find("=");
            if (index_end != -1) {
                MID_names.push_back(name.substr(index + 1, index_end - (index + 1)).c_str() + '\0');
                MID_list[MID_list.size() - 1]->name = MID_names[MID_names.size() - 1].c_str();
            }
        }
    };

    #define ADD_MID_NAME(name) name; msg::MID_AutoName UNIQUE_NAME(_unique_) = msg::MID_AutoName(#name);

    //================== All MID constants ==================

    #define _MID msg::MIDConstants::get_instance()

    class MIDConstants {

        public:
            MIDConstants() {
                mid_constant = NULL;
            }

            static MIDConstants* get_instance() {
                if (mid_constant == NULL) mid_constant = new MIDConstants();
                return mid_constant;
            }

            /*  MID syntax note :
            MID_SEND_XXX - a message to send to a client
            MID_RECV_XXX - a message to receive from a client
            MID_XXX - a message that can be both sent and received to and from clients
            */

            ADD_MID_NAME(CMID UNKNOWN = new MID(0));

            ADD_MID_NAME(CMID RECV_ID = new MID(1, FT_INT));
            ADD_MID_NAME(CMID SEND_LOGIN_USER_PASS = new MID(2, FT_CHAR_ARRAY, FT_CHAR_ARRAY));
            ADD_MID_NAME(CMID SEND_REGISTER_USER_PASS = new MID(2, FT_CHAR_ARRAY, FT_CHAR_ARRAY));

            ADD_MID_NAME(CMID BEGIN_RELAY_TEST = new MID(0));
            ADD_MID_NAME(CMID RELAY_TEST = new MID(4, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT, FT_UNSIGNED_SHORT));
            ADD_MID_NAME(CMID UDP_PING_PONG = new MID(0));

            ADD_MID_NAME(CMID RECV_UDP_PEER_BIND_REQUEST = new MID(4, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT, FT_UNSIGNED_SHORT));
            ADD_MID_NAME(CMID SEND_UDP_PEER_BIND_PORT = new MID(4, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT, FT_UNSIGNED_SHORT));
            ADD_MID_NAME(CMID SEND_PEER_CONNECT_SUCCESS = new MID(4, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT, FT_UNSIGNED_SHORT));

            ADD_MID_NAME(CMID RECV_PEER_JOIN = new MID(3, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT));
            ADD_MID_NAME(CMID RECV_PEER_LEAVE = new MID(3, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT));
            ADD_MID_NAME(CMID SEND_PEER_LEAVE = new MID(3, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT));

            ADD_MID_NAME(CMID RECV_SERVER_BINDED_UDP_PORT = new MID(1, FT_UNSIGNED_SHORT));
            ADD_MID_NAME(CMID SEND_CLIENT_BINDED_UDP_PORT = new MID(1, FT_UNSIGNED_SHORT));

        private:
            static MIDConstants* mid_constant;
    };

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