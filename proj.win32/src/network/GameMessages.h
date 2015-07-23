#ifndef _GAME_MESSAGES_H_
#define _GAME_MESSAGES_H_

#include <thread>
#include "sockets/SocketPoll.h"

namespace msg {

    namespace game {

        extern std::thread msgs_thread;
        extern SocketPoll server_poll;
        extern bool accepting_peers;

        extern void start_recv_thread();
        extern void close_all_threads();
    }
}

#endif