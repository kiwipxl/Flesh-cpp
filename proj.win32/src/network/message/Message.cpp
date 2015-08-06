#include "Message.h"

#include "../../debug/Logger.h"
#include "../sockets/Socket.h"
#include "Callback.h"

template <typename T> inline int sprintf_buf(int offset, CFTYPE t, int n) {
    return sprintf(msg::print_buf + offset, t->printf_format, *(T*)msg::last_param_list[n]->data);
}

namespace msg {

    const int MAX_PRINT_BUF = 1024;
    char print_buf[MAX_PRINT_BUF];

    void init() {
        MID_init();
    }

    void send(Socket& sock, Stream& stream) {
        //not thread safe, will crash if params are used in another thread
        //todo: param lists can be moved innto MID class to fix
        /*if (print_output || write_to_file) {
            CMID mid = extract_mid(byte_buffer, byte_offset);
            extract_params(mid, byte_buffer, byte_offset);
            std::string temp = last_MID_to_string();
            if (print_output) log_print << "sent mid: " << temp;
            if (write_to_file) log_file << "sent mid: " << temp;
            clear_param_list();
        }*/

        sock.s_send(byte_buffer, byte_offset);
    }
    
    MessagePtr extract_message(char* buffer, int buffer_len) {
        MessagePtr message = MessagePtr(new Message());
        extract_mid(message, buffer, buffer_len);
        if (message->mid->id != MID_UNKNOWN && buffer_len >= MSG_HEADER_SIZE) {
            extract_param_types(message, buffer, buffer_len);
            extract_params(message, buffer, buffer_len);
        }
        return message;
    }
    
    void extract_mid(MessagePtr message, char* buffer, int buffer_len) {
        message->mid = MID_list[0];
	    if (buffer_len >= MSG_HEADER_SIZE) {
            int id = 0;
		    memcpy(&id, buffer, 4);
		    if (id >= 0 && id < MID_list.size()) message->mid = MID_list[id];
	    }
    }
    
    void extract_param_types(MessagePtr message, char* buffer, int buffer_len) {

    }

    void extract_params(MessagePtr message, char* buffer, int buffer_len) {
	    if (message->mid->id != MID_UNKNOWN && buffer_len - MSG_HEADER_SIZE >= message->param_total_bytes) {
            int offset = MSG_HEADER_SIZE;
            for (int n = 0; n < message->types.size(); ++n) {
			    int len = 0;
			    char* pointer;
			    if (message->types[n] == FT_CHAR_ARRAY) {
                    for (int c = offset; c < buffer_len; ++c) {
                        ++len;
					    if (buffer[c] == '\0') break;
				    }
			    }else {
                    len = message->types[n]->len;
			    }
                pointer = new char[len];
                memcpy(pointer, buffer + offset, len);

                Param* p = new Param();
                p->data = pointer;
                p->len = len;
                message->params.push_back(p);

			    offset += len;
		    }
	    }
    }

    void print_extracted_params(bool print_output, bool write_to_file) {
        std::string temp = last_MID_to_string();
        if (print_output) log_print << temp;
        if (write_to_file) log_file << temp;
    }

    std::string last_MID_to_string() {
     //   if (last_MID->num_params != last_param_list_size) {
     //       log_warning << "could not print params, required " << last_MID->num_params << " params, but " << last_param_list_size << " params given";
     //       last_MID = _MID->UNKNOWN;
     //   }

	    //static const char header[] = ": ";
	    //int header_size = sizeof(header);

	    //const char* MID_name = get_MID_name(last_MID);
	    //int MID_name_len = strlen(MID_name) - 1;
     //   strcpy(print_buf, MID_name);

     //   int offset = MID_name_len - 1;
     //   if (last_MID != _MID->UNKNOWN && last_param_list_size > 0) {
     //       strcpy(print_buf + MID_name_len, header);
     //       offset += header_size;
		   // for (int n = 0; n < last_param_list_size; ++n) {
			  //  //unsure if the below code can be shortened in c++, but this is a quick work around for now at least
			  //  //sprintf requires that arguments be the same type of the format specifier, but the type is variable
			  //  CFTYPE t = last_MID->ft_params[n];
			  //  int len;

			  //  offset += sprintf(print_buf + offset, "(%s): ", t->type_name);

			  //  if (t == FT_INT)
     //               len = sprintf_buf<int>(offset, t, n);
			  //  else if (t == FT_UNSIGNED_INT)
     //               len = sprintf_buf<unsigned int>(offset, t, n);
			  //  else if (t == FT_SHORT)
     //               len = sprintf_buf<short>(offset, t, n);
			  //  else if (t == FT_UNSIGNED_SHORT)
     //               len = sprintf_buf<unsigned short>(offset, t, n);
			  //  else if (t == FT_LONG)
     //               len = sprintf_buf<long>(offset, t, n);
			  //  else if (t == FT_UNSIGNED_LONG)
     //               len = sprintf_buf<unsigned long>(offset, t, n);
			  //  else if (t == FT_FLOAT)
     //               len = sprintf_buf<float>(offset, t, n);
			  //  else if (t == FT_DOUBLE)
     //               len = sprintf_buf<double>(offset, t, n);
			  //  else if (t == FT_BOOL)
     //               len = sprintf_buf<bool>(offset, t, n);
			  //  else if (t == FT_CHAR_ARRAY)
     //               len = sprintf_buf<char*>(offset, t, n);
			  //  else
				 //   len = sprintf(print_buf + offset, "%s", "undefined");

			  //  offset += len;
			  //  if (n < last_param_list_size - 1) offset += sprintf(print_buf + offset, ", ", t->type_name);
		   // }
     //   }
     //   print_buf[offset + 1] = '\0';

        return print_buf;
    }

    inline const char* get_MID_name(CMID mid) {
	    return (MID_list.size() > 0 && mid->id > 0 && mid->id < MID_list.size()) ? MID_list[mid->id]->name : "undefined";
    }
};
