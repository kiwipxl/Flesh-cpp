#include "Message.h"

extern void Msg::send(Socket* sock, ByteStream& stream) {
	sock->send_buffer(Msg::byte_buffer, Msg::byte_offset);
}