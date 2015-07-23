#include "SocketPoll.h"

int SocketPoll::poll() {
    if (fds.size() <= 0) return 0;
    return Socket::poll_fds(&fds[0], fds.size(), 1000);
}

void SocketPoll::add_sock(Socket& sock) {
    pollfd* fd = new pollfd();
    fd->fd = sock.get_sock();
    fd->events = POLLRDNORM | POLLRDBAND;
    fds.push_back(*fd);
    sockets.push_back(&sock);
}

void SocketPoll::remove_sock(Socket& sock) {
    for (int n = 0; n < sockets.size(); ++n) {
        if (sockets[n] == &sock) {
            sockets.erase(sockets.begin() + n, sockets.end() + n + 1);
            fds.erase(fds.begin() + n, fds.end() + n + 1);
        }
    }
}

void SocketPoll::remove_sock(int at) {
    for (int n = 0; n < sockets.size(); ++n) {
        if (n == at) {
            sockets.erase(sockets.begin() + n, sockets.end() + n + 1);
            fds.erase(fds.begin() + n, fds.end() + n + 1);
        }
    }
}