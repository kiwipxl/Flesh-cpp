#ifndef _GAME_MESSAGES_H_
#define _GAME_MESSAGES_H_

#include <thread>
#include "SocketPoll.h"

namespace msgs {

    extern std::thread game_msgs_thread;

    extern void start_recv_game_msgs();
}

#endif