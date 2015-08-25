#ifndef _GAME_MESSAGES_H_
#define _GAME_MESSAGES_H_

#include <thread>

#include "network/Defines.h"

BEGIN_NETWORK_NS

namespace msg {

    extern std::thread msgs_thread;
    extern bool accepting_peers;

    extern void add_poll_sock(sock::Socket& sock);

    extern void start_recv_thread();
    extern void close_all_threads();
};

END_NETWORK_NS

#endif