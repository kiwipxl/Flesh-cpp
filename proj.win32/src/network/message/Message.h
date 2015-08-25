#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "debug/Logger.h"
#include "network/Defines.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

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
    sock::Socket* sock = NULL;

    Message();

    template <typename T> T get(int index) {
        if (index < 0 || index >= params.size()) f_assert("index is out of bounds of message parameters");
        return *(T*)params[index]->data;
    }
        
    template <> char* get<char*>(int index) {
        if (index < 0 || index >= params.size()) f_assert("index is out of bounds of message parameters");
        return params[index]->data;
    }

    template <> GeneralResult get<GeneralResult>(int index) {
        if (index < 0 || index >= params.size()) f_assert("index is out of bounds of message parameters");
        return (GeneralResult)*(u_short*)params[index]->data;
    }
};

#define MSG_HEADER_SIZE 4

extern const int MAX_NUM_PARAMS;
extern const int MAX_PRINT_BUF;
extern char print_buf[];

void init();
void send(sock::Socket& sock, Stream& stream);
    
MessagePtr extract_message(sock::Socket& sock, char* buffer, int buffer_len);
void extract_mid(MessagePtr message, char* buffer, int buffer_len);
void extract_params(MessagePtr message, char* buffer, int buffer_len);

void print_extracted_params(bool print_output = true, bool write_to_file = false);
std::string last_MID_to_string();

END_MSG_NS
END_NETWORK_NS

#endif