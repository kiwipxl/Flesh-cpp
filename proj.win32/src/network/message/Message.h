#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <sstream>

#include "../sockets/Socket.h"
#include "MID.h"
#include "MsgStream.h"

namespace msg {

	//================== Parameters begin ==================

	struct Param {

		char* data;
		int len;
	};

	extern const int MAX_NUM_PARAMS;
	extern const int MAX_PRINT_BUF;
    extern char print_buf[];

    extern CMID last_MID;
    extern u_short last_callback_id;
	extern Param* last_param_list[];
	extern int last_param_tbytes;
	extern int last_param_list_size;

	void init();
    void send(Socket& sock, ByteStream& stream, std::function<void()> callback = NULL);
    
    void extract_msg(char* buffer, int buffer_len);
	CMID extract_mid(char* buffer, int buffer_len);
    u_short extract_callback_id(char* buffer, int buffer_len);
	void extract_params(CMID mid, char* byte_data, int byte_data_len);
	void clear_param_list();

    void print_extracted_params(bool print_output = true, bool write_to_file = false);
    std::string last_MID_to_string();
    inline const char* get_MID_name(CMID mid);
}

#endif