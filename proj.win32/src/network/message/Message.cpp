#include "Message.h"

#include "../../debug/Logger.h"

//the following namespace inclusions are used to avoid redundant "msg::" accesses
using msg::MID;
using msg::FormatType;
using msg::Param;

CMID msg::last_MID;
u_short msg::last_callback_id = 0;
const int msg::MAX_NUM_PARAMS = 16;
Param* msg::last_param_list[MAX_NUM_PARAMS];
int msg::last_param_list_size = 0;
int msg::last_param_tbytes = 0;

const int msg::MAX_PRINT_BUF = 1024;
char msg::print_buf[MAX_PRINT_BUF];

void msg::init() {
	for (int n = 0; n < MAX_NUM_PARAMS; ++n) {
        last_param_list[n] = new Param();
	}
}

void msg::send(Socket& sock, MsgStream& stream, std::function<void()> callback) {
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
    sock.add_unique_id_callback(callback, stream.mid, stream.mid->callback_id_inc);
}

void msg::extract_msg(char* buffer, int buffer_len) {
    CMID mid = extract_mid(buffer, buffer_len);
    if (mid != _MID->UNKNOWN && buffer_len >= MSG_HEADER_SIZE) {
        extract_callback_id(buffer, buffer_len);
        extract_params(mid, buffer, buffer_len);
    }
}

CMID msg::extract_mid(char* buffer, int buffer_len) {
	CMID mid = _MID->UNKNOWN;
	if (buffer_len >= 4) {
		int id = 0;
		memcpy(&id, buffer, 4);
		if (id >= 0 && id < MID_list.size()) mid = MID_list[id];
	}
    last_MID = mid;
    return mid;
}

u_short msg::extract_callback_id(char* buffer, int buffer_len) {
    int callback_id = 0;
    if (buffer_len >= MSG_HEADER_SIZE) {
        memcpy(&callback_id, buffer + 4, 4);
    }
    return callback_id;
}

std::string concat_str = "";
void msg::extract_params(CMID mid, char* byte_data, int byte_data_len) {
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

void msg::clear_param_list() {
	for (int n = 0; n < last_param_list_size; ++n) {
		if (last_param_list[n] != NULL) delete[] last_param_list[n]->data;
	}
	last_param_list_size = 0;
	last_param_tbytes = 0;
}

void msg::print_extracted_params(bool print_output, bool write_to_file) {
    std::string temp = last_MID_to_string();
    if (print_output) log_print << temp;
    if (write_to_file) log_file << temp;
}

std::string msg::last_MID_to_string() {
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
				len = sprintf(print_buf + offset, t->printf_format, *(int*)last_param_list[n]->data);
			else if (t == FT_UNSIGNED_INT)
				len = sprintf(print_buf + offset, t->printf_format, *(unsigned int*)last_param_list[n]->data);
			else if (t == FT_SHORT)
				len = sprintf(print_buf + offset, t->printf_format, *(short*)last_param_list[n]->data);
			else if (t == FT_UNSIGNED_SHORT)
				len = sprintf(print_buf + offset, t->printf_format, *(unsigned short*)last_param_list[n]->data);
			else if (t == FT_LONG)
				len = sprintf(print_buf + offset, t->printf_format, *(long*)last_param_list[n]->data);
			else if (t == FT_UNSIGNED_LONG)
				len = sprintf(print_buf + offset, t->printf_format, *(unsigned long*)last_param_list[n]->data);
			else if (t == FT_FLOAT)
				len = sprintf(print_buf + offset, t->printf_format, *(float*)last_param_list[n]->data);
			else if (t == FT_DOUBLE)
				len = sprintf(print_buf + offset, t->printf_format, *(double*)last_param_list[n]->data);
			else if (t == FT_BOOL)
				len = sprintf(print_buf + offset, t->printf_format, *(bool*)last_param_list[n]->data);
			else if (t == FT_CHAR_ARRAY)
				len = sprintf(print_buf + offset, t->printf_format, last_param_list[n]->data);
			else
				len = sprintf(print_buf + offset, "%s", "undefined");

			offset += len;
			if (n < last_param_list_size - 1) offset += sprintf(print_buf + offset, ", ", t->type_name);
		}
    }
    print_buf[offset + 1] = '\0';

    return print_buf;
}

inline const char* msg::get_MID_name(CMID mid) {
	return (MID_names.size() > 0 && mid->id > 0 && mid->id < MID_names.size()) ? MID_names[mid->id].c_str() : "undefined";
}