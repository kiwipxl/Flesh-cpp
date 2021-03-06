#include "debug/Errors.h"

#include "network/sockets/Socket.h"

int debug::fresult;

int debug::get_last_error() {
	#ifdef PLATFORM_WIN32
		return WSAGetLastError();
	#elif defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)
		return errno;
	#endif
}