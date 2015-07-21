#ifndef _GAME_MESSAGES_H_
#define _GAME_MESSAGES_H_

#include <thread>
#include "SocketPoll.h"

namespace game_msgs {

    extern std::thread msgs_thread;
    extern SocketPoll server_poll;

    extern void start_recv_thread();
}

#endif