#include "Message.h"

using message::MID;
using message::FormatType;

FormatType::FormatType(char c, short l) : chr(c), len(l) { }

//format types for packing and unpacking byte data
CFTYPE message::FT_CHAR								= new FormatType('c', 1);
CFTYPE message::FT_SIGNED_CHAR						= new FormatType('b', 1);
CFTYPE message::FT_UNSIGNED_CHAR					= new FormatType('B', 1);
CFTYPE message::FT_BOOL								= new FormatType('?', 1);
CFTYPE message::FT_SHORT							= new FormatType('h', 2);
CFTYPE message::FT_UNSIGNED_SHORT					= new FormatType('H', 2);
CFTYPE message::FT_INT								= new FormatType('i', 4);
CFTYPE message::FT_UNSIGNED_INT						= new FormatType('I', 4);
CFTYPE message::FT_LONG								= new FormatType('l', 8);
CFTYPE message::FT_UNSIGNED_LONG					= new FormatType('L', 8);
CFTYPE message::FT_LONG_LONG						= new FormatType('q', 8);
CFTYPE message::FT_UNSIGNED_LONG_LONG				= new FormatType('Q', 8);
CFTYPE message::FT_FLOAT							= new FormatType('f', 4);
CFTYPE message::FT_DOUBLE							= new FormatType('d', 8);
CFTYPE message::FT_CHAR_ARRAY						= new FormatType('s', 1);
CFTYPE message::FT_VOID_POINTER						= new FormatType('p', 4);

int message::MID_id = 0;
std::vector<message::MID*> message::MID_list;

CMID message::MID_UNKNOWN							= new MID(0);
CMID message::MID_CLIENT_ID							= new MID(1, message::FT_INT);
CMID message::MID_CLIENT_USER_PASS					= new MID(4, message::FT_BOOL, message::FT_BOOL, message::FT_INT, message::FT_FLOAT);

MID::MID(int num_args, ...) {
	id = MID_id;
	if (num_args > 0) ft_params = new CFTYPE[num_args];
	num_params = num_args;

	va_list ap;
	va_start(ap, num_args);
	for (int n = 0; n < num_args; ++n) {
		ft_params[n] = va_arg(ap, CFTYPE);
		total_param_bytes += ft_params[n]->len;
	}
	va_end(ap);

	++MID_id;
	MID_list.push_back(this);
}

char message::byte_buffer[1024];
int message::byte_offset;
std::vector<char*> message::param_list = std::vector<char*>();

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
	return mid;
}

void message::extract_params(CMID mid, char* byte_data, int byte_data_len) {
	clear_param_list();
	if (mid != MID_UNKNOWN && byte_data_len - 4 >= mid->total_param_bytes) {
		int offset = 4;
		for (int n = 0; n < mid->num_params; ++n) {
			if (mid->ft_params[n] == FT_CHAR_ARRAY) {
			}else {
				int len = mid->ft_params[n]->len;
				char* pointer = new char[len];
				memcpy(pointer, byte_data + offset, len);
				param_list.push_back(pointer);
				offset += len;
			}
		}
	}
}

void message::clear_param_list() {
	if (param_list.size() > 0) {
		message::param_list.clear();
	}
}