#include "network/message/MessagePoller.h"

#include "utility/Logger.h"
#include "entities/units/Unit.h"
#include "network/message/Message.h"
#include "network/Peers.h"
#include "network/message/MID.h"
#include "network/sockets/SocketPoll.h"
#include "network/message/Callback.h"
#include "network/sockets/Socket.h"
#include "network/server/ServerConnection.h"
#include "StateManager.h"

BEGIN_NETWORK_NS
BEGIN_MSG_NS

//private
std::thread recv_thread;
std::mutex recv_mutex;
bool recv_thread_running = false;
bool thread_done = false;

char buffer[1024];
int msg_len;

//public
void close_thread() {
    recv_thread_running = false;
    //temporary thread fix
    Sleep(500);
}

void poll(sock::SocketPoll& p) {
    process_all_callbacks(p);

	int total = 0;
	if ((total = p.poll()) > 0) {
        int size = p.get_size();
        for (int i = 0; i < size; ++i) {
            if (p.get_size() != size) break;
            int revents = p.get_fd_at(i)->revents;
			if (revents & POLLERR) {
                log_warning << "poll error occurred";
			}else if (revents & POLLHUP) {
                log_warning << "poll hang up error occurred";
			}else if (revents & POLLNVAL) {
                log_warning << "poll invalid request occurred";
			}else if (revents & POLLRDNORM || revents & POLLRDBAND || revents & POLLIN || revents & POLLPRI) {
                if (p.get_size() != size) break;
                sock::Socket* sock = p.get_sock_at(i);
                if (!sock) continue;
                peers::Peer* peer;

				if ((msg_len = sock->s_recv(buffer, 1024)) > 0) {
                    MessagePtr message = extract_message(*sock, buffer, msg_len);
					if (message->mid->id != MID_UNKNOWN) {
                        process_message_callback(message);

                        print_extracted_params(false, true);
					}
				}
			}
		}
	}else if (total == -1) {
        log_error << "polling error occurred: " << debug::get_last_error();
	}
}

void start_recv_thread() {
    close_thread();

    recv_thread = std::thread([]() {
        recv_mutex.lock();
        recv_thread_running = true;
        thread_done = false;

        while (true) {
            if (!recv_thread_running) break;

            poll(server::get_poll());
        }

        thread_done = true;

        recv_mutex.unlock();
    });
    recv_thread.detach();
    bool j = recv_thread.joinable();
    int s = 0;
}

bool is_recv_thread_running() { return recv_thread_running; }

END_MSG_NS
END_NETWORK_NS