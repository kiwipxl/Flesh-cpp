#include "Message.h"

#include "../../debug/Logger.h"

template <typename T> inline int sprintf_buf(int offset, CFTYPE t, int n) {
    return sprintf(msg::print_buf + offset, t->printf_format, *(T*)msg::last_param_list[n]->data);
}

namespace msg {

    CMID last_MID;
    u_short last_callback_id = 0;
    const int MAX_NUM_PARAMS = 16;
    Param* last_param_list[MAX_NUM_PARAMS];
    int last_param_list_size = 0;
    int last_param_tbytes = 0;
    ResponseCode last_response_code = RESPONSE_NONE;

    const int MAX_PRINT_BUF = 1024;
    char print_buf[MAX_PRINT_BUF];

    void init() {
        for (int n = 0; n < MAX_NUM_PARAMS; ++n) {
            last_param_list[n] = new Param();
        }
    }

    void send(Socket& sock, MsgStream& stream, MsgCallbackPtr callback) {
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
        if (callback != nullptr) sock.add_callback(callback);
    }
    
    MessagePtr extract_message(char* buffer, int buffer_len) {
        MessagePtr msg_ptr = MessagePtr(new Message());
        extract_mid(msg_ptr, buffer, buffer_len);
        if (msg_ptr->mid->id != MID_UNKNOWN && buffer_len >= MSG_HEADER_SIZE) {
            extract_param_types(msg_ptr, buffer, buffer_len);
            extract_params(msg_ptr, buffer, buffer_len);
        }
    }
    
    void extract_mid(MessagePtr message_ptr, char* buffer, int buffer_len) {
        message_ptr->mid = MID_list[0];
	    if (buffer_len >= MSG_HEADER_SIZE) {
            int id = 0;
		    memcpy(&id, buffer, 4);
		    if (id >= 0 && id < MID_list.size()) message_ptr->mid = MID_list[id];
	    }
    }
    
    void extract_param_types(MessagePtr message_ptr, char* buffer, int buffer_len) {

    }

    std::string concat_str = "";
    void extract_params(MessagePtr message_ptr, char* buffer, int buffer_len) {
	    clear_param_list();

	    if (mid != _MID->UNKNOWN && byte_data_len - MSG_HEADER_SIZE >= mid->total_param_bytes) {
            int offset = MSG_HEADER_SIZE;
		    int index = 0;
		    for (int n = 0; n < mid->num_params; ++n) {
			    int len = 0;
			    char* pointer;
			    if (mid->ft_params[n] == FT_CHAR_ARRAY) {
				    concat_str = "";
				    for (int c = offset; c < byte_data_len; ++c) {
					    ++len;
					    concat_str += byte_data[c];
					    if (byte_data[c] == '\0') break;
				    }
				    pointer = new char[len];
				    strcpy(pointer, concat_str.c_str());
			    }else {
				    len = mid->ft_params[n]->len;
				    pointer = new char[len];
				    memcpy(pointer, byte_data + offset, len);
			    }
			    last_param_list[index]->data = pointer;
			    last_param_list[index]->len = len;
			    offset += len;
			    last_param_tbytes += len;
			    ++index;
		    }
		    last_param_list_size = index;
	    }
    }

    void clear_param_list() {
	    for (int n = 0; n < last_param_list_size; ++n) {
		    if (last_param_list[n] != NULL) delete[] last_param_list[n]->data;
	    }
	    last_param_list_size = 0;
	    last_param_tbytes = 0;
    }

    void print_extracted_params(bool print_output, bool write_to_file) {
        std::string temp = last_MID_to_string();
        if (print_output) log_print << temp;
        if (write_to_file) log_file << temp;
    }

    std::string last_MID_to_string() {
        if (last_MID->num_params != last_param_list_size) {
            log_warning << "could not print params, required " << last_MID->num_params << " params, but " << last_param_list_size << " params given";
            last_MID = _MID->UNKNOWN;
        }

	    static const char header[] = ": ";
	    int header_size = sizeof(header);

	    const char* MID_name = get_MID_name(last_MID);
	    int MID_name_len = strlen(MID_name) - 1;
        strcpy(print_buf, MID_name);

        int offset = MID_name_len - 1;
        if (last_MID != _MID->UNKNOWN && last_param_list_size > 0) {
            strcpy(print_buf + MID_name_len, header);
            offset += header_size;
		    for (int n = 0; n < last_param_list_size; ++n) {
			    //unsure if the below code can be shortened in c++, but this is a quick work around for now at least
			    //sprintf requires that arguments be the same type of the format specifier, but the type is variable
			    CFTYPE t = last_MID->ft_params[n];
			    int len;

			    offset += sprintf(print_buf + offset, "(%s): ", t->type_name);

			    if (t == FT_INT)
                    len = sprintf_buf<int>(offset, t, n);
			    else if (t == FT_UNSIGNED_INT)
                    len = sprintf_buf<unsigned int>(offset, t, n);
			    else if (t == FT_SHORT)
                    len = sprintf_buf<short>(offset, t, n);
			    else if (t == FT_UNSIGNED_SHORT)
                    len = sprintf_buf<unsigned short>(offset, t, n);
			    else if (t == FT_LONG)
                    len = sprintf_buf<long>(offset, t, n);
			    else if (t == FT_UNSIGNED_LONG)
                    len = sprintf_buf<unsigned long>(offset, t, n);
			    else if (t == FT_FLOAT)
                    len = sprintf_buf<float>(offset, t, n);
			    else if (t == FT_DOUBLE)
                    len = sprintf_buf<double>(offset, t, n);
			    else if (t == FT_BOOL)
                    len = sprintf_buf<bool>(offset, t, n);
			    else if (t == FT_CHAR_ARRAY)
                    len = sprintf_buf<char*>(offset, t, n);
			    else
				    len = sprintf(print_buf + offset, "%s", "undefined");

			    offset += len;
			    if (n < last_param_list_size - 1) offset += sprintf(print_buf + offset, ", ", t->type_name);
		    }
        }
        print_buf[offset + 1] = '\0';

        return print_buf;
    }

    inline const char* get_MID_name(CMID mid) {
	    return (MID_names.size() > 0 && mid->id > 0 && mid->id < MID_names.size()) ? MID_names[mid->id].c_str() : "undefined";
    }
};
