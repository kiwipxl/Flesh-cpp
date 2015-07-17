#include "Message.h"

using message::MID;
using message::FormatType;

FormatType::FormatType(char c, short l) : chr(c), len(l) { }

//format types for packing and unpacking byte data
const FormatType message::FT_CHAR					= FormatType('c', 1);
const FormatType message::FT_SIGNED_CHAR			= FormatType('b', 1);
const FormatType message::FT_UNSIGNED_CHAR			= FormatType('B', 1);
const FormatType message::FT_BOOL					= FormatType('?', 1);
const FormatType message::FT_SHORT					= FormatType('h', 2);
const FormatType message::FT_UNSIGNED_SHORT			= FormatType('H', 2);
const FormatType message::FT_INT					= FormatType('i', 4);
const FormatType message::FT_UNSIGNED_INT			= FormatType('I', 4);
const FormatType message::FT_LONG					= FormatType('l', 8);
const FormatType message::FT_UNSIGNED_LONG			= FormatType('L', 8);
const FormatType message::FT_LONG_LONG				= FormatType('q', 8);
const FormatType message::FT_UNSIGNED_LONG_LONG		= FormatType('Q', 8);
const FormatType message::FT_FLOAT					= FormatType('f', 4);
const FormatType message::FT_DOUBLE					= FormatType('d', 8);
const FormatType message::FT_CHAR_ARRAY				= FormatType('s', 0);
const FormatType message::FT_VOID_POINTER			= FormatType('p', 4);

int message::MID_id = 0;
std::vector<message::MID*> message::MID_list;

CMID message::MID_UNKNOWN							= new MID(0);
CMID message::MID_CLIENT_ID							= new MID(1, message::FT_INT);
CMID message::MID_CLIENT_USER_PASS					= new MID(2, message::FT_CHAR_ARRAY, message::FT_CHAR_ARRAY);

MID::MID(int num_args, ...) {
	id = MID_id;
	if (num_args > 0) ft_params = new FormatType[num_args];
	num_params = num_args;

	va_list ap;
	va_start(ap, num_args);
	for (int n = 0; n < num_args; ++n) {
		ft_params[n] = va_arg(ap, FormatType);
		total_param_bytes = ft_params[n].len;
	}
	va_end(ap);

	++MID_id;
	MID_list.push_back(this);
}

char message::byte_buffer[1024];
int message::byte_offset;
std::vector<void*> message::param_list = std::vector<void*>();

void message::send(Socket* sock, ByteStream& stream) {
	sock->s_send(byte_buffer, byte_offset);
}

CMID message::extract_mid(char* buffer, int buffer_len) {
	CMID mid = message::MID_UNKNOWN;
	if (buffer_len >= 4) {
		int id = 0;
		memcpy(&id, buffer, 4);
		if (id >= 0 && id < MID_list.size()) mid = MID_list[id];
	}
	return mid;
}

void message::extract_params(CMID mid, char* byte_data, int byte_data_len) {
	message::param_list.clear();
	if (mid != message::MID_UNKNOWN && byte_data_len - 4 >= mid->total_param_bytes) {
		int offset = 0;
		for (int n = 0; n < mid->num_params; ++n) {
			if (mid->ft_params[n] == message::FT_CHAR_ARRAY) {
			}else {
			}
		}
	}
}

void message::unpack(void* dest, char* buffer, int buffer_len) {
	memcpy(dest, buffer, buffer_len);
}
