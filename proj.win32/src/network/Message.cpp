#include "Message.h"

//the following namespace inclusions are used to avoid redundant "message::" accesses
using message::MID;
using message::FormatType;
using message::Param;

using message::FT_CHAR;
using message::FT_SIGNED_CHAR;
using message::FT_UNSIGNED_CHAR;
using message::FT_BOOL;
using message::FT_SHORT;
using message::FT_UNSIGNED_SHORT;
using message::FT_INT;
using message::FT_UNSIGNED_INT;
using message::FT_LONG;
using message::FT_UNSIGNED_LONG;
using message::FT_LONG_LONG;
using message::FT_UNSIGNED_LONG_LONG;
using message::FT_FLOAT;
using message::FT_DOUBLE;
using message::FT_CHAR_ARRAY;
using message::FT_VOID_POINTER;

FormatType::FormatType(const char* c, const short l, const char* name) : printf_format(c), len(l), type_name(name) { }

//format types for packing and unpacking byte data
CFTYPE message::FT_CHAR								= new FormatType("%c", 1, "char");
CFTYPE message::FT_SIGNED_CHAR						= new FormatType("%c", 1, "schar");
CFTYPE message::FT_UNSIGNED_CHAR					= new FormatType("%c", 1, "uchar");
CFTYPE message::FT_BOOL								= new FormatType("%d", 1, "bool");
CFTYPE message::FT_SHORT							= new FormatType("%d", 2, "short");
CFTYPE message::FT_UNSIGNED_SHORT					= new FormatType("%d", 2, "ushort");
CFTYPE message::FT_INT								= new FormatType("%i", 4, "int");
CFTYPE message::FT_UNSIGNED_INT						= new FormatType("%u", 4, "uint");
CFTYPE message::FT_LONG								= new FormatType("%li", 8, "long");
CFTYPE message::FT_UNSIGNED_LONG					= new FormatType("%lu", 8, "ulong");
CFTYPE message::FT_LONG_LONG						= new FormatType("%lli", 8, "llong");
CFTYPE message::FT_UNSIGNED_LONG_LONG				= new FormatType("%llu", 8, "ullong");
CFTYPE message::FT_FLOAT							= new FormatType("%f", 4, "float");
CFTYPE message::FT_DOUBLE							= new FormatType("%f", 8, "double");
CFTYPE message::FT_CHAR_ARRAY						= new FormatType("%s", 1, "char*");
CFTYPE message::FT_VOID_POINTER						= new FormatType("%p", 4, "void*");

int message::MID_id = 0;
std::vector<CMID> message::MID_list;
std::vector<std::string> message::MID_names;

ADD_MID_NAME(CMID message::MID_UNKNOWN					= new MID(0));
ADD_MID_NAME(CMID message::MID_CLIENT_ID				= new MID(5, FT_INT, FT_CHAR_ARRAY, FT_CHAR_ARRAY, FT_INT, FT_CHAR_ARRAY));
ADD_MID_NAME(CMID message::MID_CLIENT_USER_PASS			= new MID(2, FT_CHAR_ARRAY, FT_CHAR_ARRAY));
ADD_MID_NAME(CMID message::MID_BEGIN_RELAY_TEST			= new MID(0));
ADD_MID_NAME(CMID message::MID_RELAY_TEST				= new MID(3, FT_INT, FT_CHAR_ARRAY, FT_UNSIGNED_SHORT));
ADD_MID_NAME(CMID message::MID_GET_TCP_CLIENT_PORT		= new MID(1, FT_UNSIGNED_SHORT));

MID::MID(int num_args, ...) : id(MID_id) {
	if (num_args > 0) ft_params = new CFTYPE[num_args];
	num_params = num_args;

	va_list ap;
	va_start(ap, num_args);
	for (int n = 0; n < num_args; ++n) {
		CFTYPE ft = va_arg(ap, CFTYPE); 
		ft_params[n] = ft;
		total_param_bytes += ft->len;
	}
	va_end(ap);

	++MID_id;
	MID_list.push_back(this);
}

char message::byte_buffer[1024];
int message::byte_offset;

const int message::MAX_NUM_PARAMS = 16;
Param* message::param_list[MAX_NUM_PARAMS];
int message::param_list_size = 0;
int message::param_tbytes = 0;
CMID message::last_extracted_mid;
const int message::MAX_PRINT_BUF = 1024;
char message::print_buf[MAX_PRINT_BUF];

void message::init() {
	for (int n = 0; n < MAX_NUM_PARAMS; ++n) {
		param_list[n] = new Param();
	}
}

void message::send(Socket* sock, ByteStream& stream) {
	sock->s_send(byte_buffer, byte_offset);
}

CMID message::extract_mid(char* buffer, int buffer_len) {
	CMID mid = MID_UNKNOWN;
	if (buffer_len >= 4) {
		int id = 0;
		memcpy(&id, buffer, 4);
		if (id >= 0 && id < MID_list.size()) mid = MID_list[id];
	}
	last_extracted_mid = mid;
	return mid;
}

std::string concat_str = "";
void message::extract_params(CMID mid, char* byte_data, int byte_data_len) {
	clear_param_list();

	if (mid != MID_UNKNOWN && byte_data_len - 4 >= mid->total_param_bytes) {
		int offset = 4;
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
			param_list[index]->data = pointer;
			param_list[index]->len = len;
			offset += len;
			param_tbytes += len;
			++index;
		}
		param_list_size = index;
	}
}

void message::clear_param_list() {
	for (int n = 0; n < param_list_size; ++n) {
		if (param_list[n] != NULL) delete[] param_list[n]->data;
	}
	param_list_size = 0;
	param_tbytes = 0;
}

void message::print_extracted_params() {
	if (last_extracted_mid->num_params == param_list_size) {
		static const char header[] = ": ";
		int header_size = sizeof(header);
		const char* MID_name = get_MID_name(last_extracted_mid);
		int MID_name_len = strlen(MID_name) - 1;
		strcpy(print_buf, MID_name);
		strcpy(print_buf + MID_name_len, header);
		int offset = header_size + MID_name_len - 1;
		for (int n = 0; n < param_list_size; ++n) {
			//unsure if the below code can be shortened in c++, but this is a quick work around for now at least
			//sprintf requires that arguments be the same type of the format specifier, but the type is variable
			CFTYPE t = last_extracted_mid->ft_params[n];
			int len;

			offset += sprintf(print_buf + offset, "(%s): ", t->type_name);

			if (t == FT_INT)
				len = sprintf(print_buf + offset, t->printf_format, *(int*)param_list[n]->data);
			else if (t == FT_UNSIGNED_INT)
				len = sprintf(print_buf + offset, t->printf_format, *(unsigned int*)param_list[n]->data);
			else if (t == FT_SHORT)
				len = sprintf(print_buf + offset, t->printf_format, *(short*)param_list[n]->data);
			else if (t == FT_UNSIGNED_SHORT)
				len = sprintf(print_buf + offset, t->printf_format, *(unsigned short*)param_list[n]->data);
			else if (t == FT_LONG)
				len = sprintf(print_buf + offset, t->printf_format, *(long*)param_list[n]->data);
			else if (t == FT_UNSIGNED_LONG)
				len = sprintf(print_buf + offset, t->printf_format, *(unsigned long*)param_list[n]->data);
			else if (t == FT_FLOAT)
				len = sprintf(print_buf + offset, t->printf_format, *(float*)param_list[n]->data);
			else if (t == FT_DOUBLE)
				len = sprintf(print_buf + offset, t->printf_format, *(double*)param_list[n]->data);
			else if (t == FT_BOOL)
				len = sprintf(print_buf + offset, t->printf_format, *(bool*)param_list[n]->data);
			else if (t == FT_CHAR_ARRAY)
				len = sprintf(print_buf + offset, t->printf_format, param_list[n]->data);
			else
				len = sprintf(print_buf + offset, "%s", "undefined");

			offset += len;
			if (n < param_list_size - 1) offset += sprintf(print_buf + offset, ", ", t->type_name);
		}
		print_buf[offset + 1] = '\0';
		CCLOG(print_buf, "");
	}else {
		CCLOG("could not print params, required %d params, but %d params given", last_extracted_mid->num_params, param_list_size);
	}
}

inline const char* message::get_MID_name(CMID mid) {
	return (MID_names.size() > 0 && mid->id > 0 && mid->id < MID_names.size()) ? MID_names[mid->id].c_str() : "undefined";
}