#include "Message.h"

using message::MID;
using message::FormatType;

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

const MID message::MID_UNKNOWN					= MID();
const MID message::MID_CLIENT_ID				= MID(message::FT_INT);
const MID message::MID_CLIENT_USER_PASS			= MID(message::FT_CHAR_ARRAY, message::FT_CHAR_ARRAY);

message::MID::MID(...) {
	id = MID_id;

	++MID_id;
	MID_list.push_back(this);
}

char message::byte_buffer[1024];
int message::byte_offset;

extern void message::send(Socket* sock, ByteStream& stream) {
	sock->s_send(byte_buffer, byte_offset);
}
