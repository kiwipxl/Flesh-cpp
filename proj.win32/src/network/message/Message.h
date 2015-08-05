#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <sstream>

#include "../sockets/Socket.h"
#include "MID.h"
#include "MsgStream.h"

namespace msg {

    struct Param {

        char* data;
        int len;

        template <typename ParamType> ParamType& get() {
            return *(ParamType*)data;
        }
    };

    struct Message {

        //identifier
        const MID* mid;

        //params
        std::vector<CFTYPE> types;
        std::vector<Param*> params;
        int total_bytes;
        int count;
    };

    #define MessagePtr std::shared_ptr<Message>

	extern const int MAX_NUM_PARAMS;
	extern const int MAX_PRINT_BUF;
    extern char print_buf[];

	void init();
    void send(Socket& sock, MsgStream& stream, MsgCallbackPtr callback = nullptr);
    
    MessagePtr extract_message(char* buffer, int buffer_len);
    void extract_mid(MessagePtr message_ptr, char* buffer, int buffer_len);
    void extract_param_types(MessagePtr message_ptr, char* buffer, int buffer_len);
    void extract_params(MessagePtr message_ptr, char* buffer, int buffer_len);

    void print_extracted_params(bool print_output = true, bool write_to_file = false);
    std::string last_MID_to_string();
    inline const char* get_MID_name(MID* mid);
}

#endif