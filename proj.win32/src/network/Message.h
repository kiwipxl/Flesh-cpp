#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <stdarg.h>
#include <sstream>
#include "ReadableType.h"
#include "Socket.h"

enum MID {
	MID_UNKNOWN,
	MID_CLIENT_ID,
	MID_CLIENT_USER_PASS
};

class Message {

	public:
		MID msg_id = MID_UNKNOWN;
		char* params;
		std::string raw_data;
};

namespace Msg {
	
	extern char byte_buffer[1024];
	extern int byte_offset;

	class ByteStream {

		public:
			ByteStream() { byte_offset = 0; }

			template <class T> ByteStream& operator<<(const T& v) {
				memcpy(byte_buffer + byte_offset, &v, sizeof(v));
				byte_offset += sizeof(v);
				return *this;
			}
	};

	extern void send(Socket* sock, ByteStream& stream);
}

#endif