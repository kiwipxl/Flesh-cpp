#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <sstream>

#include "MID.h"
#include "Stream.h"
#include "Callback.h"

class Socket;

namespace msg {

    struct Param {

        char* data;
        int len;

        template <typename ParamType> ParamType& get() {
            return *(ParamType*)data;
        }
    };

    struct Message {

        CMID mid;
        std::vector<Param*> params;
        CallbackResult callback_result;

        template <typename ParamType> ParamType& get(int index) {
            if (index < 0 || index >= params.size()) assert("index is out of bounds of message parameters");
            return *(ParamType*)params[index]->data;
        }
    };
    
    typedef std::shared_ptr<Message> MessagePtr;
    #define MSG_HEADER_SIZE 4

	extern const int MAX_NUM_PARAMS;
	extern const int MAX_PRINT_BUF;
    extern char print_buf[];

    void init();
    void send(Socket& sock, Stream& stream);
    
    MessagePtr extract_message(char* buffer, int buffer_len);
    void extract_mid(MessagePtr message, char* buffer, int buffer_len);
    void extract_params(MessagePtr message, char* buffer, int buffer_len);

    void print_extracted_params(bool print_output = true, bool write_to_file = false);
    std::string last_MID_to_string();
    inline const char* get_MID_name(CMID mid);
}

#endif