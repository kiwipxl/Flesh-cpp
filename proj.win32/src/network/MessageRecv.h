#ifndef _SOCKET_POLL_
#define _SOCKET_POLL_

/*

*/
#include <vector>
#include "Socket.h"

class SocketPoll {
    
    public:
        void poll();
        void add_sock(Socket* sock);
        void remove_sock(Socket* sock);

        int get_size() { return fds.size(); }
        pollfd* get_fd_at(int i) { return (i >= 0 && i < get_size()) ? &fds[i] : NULL; }
        Socket* get_sock_at(int i) { return (i >= 0 && i < get_size()) ? sockets[i] : NULL; }

    private:
        std::vector<pollfd> fds;
        std::vector<Socket*> sockets;
};

#endif