#ifndef _GAME_MESSAGES_H_
#define _GAME_MESSAGES_H_

#include <thread>

#include "network/Defines.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

extern void start_recv_thread();
extern void close_thread();

extern bool is_recv_thread_running();

END_MSG_NS
END_NETWORK_NS

#endif