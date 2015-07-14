#include "Message.h"

char Msg::byte_buffer[1024];
int Msg::byte_offset;

extern void Msg::send(Socket* sock, ByteStream& stream) {
	sock->send_buffer(byte_buffer, byte_offset);
}