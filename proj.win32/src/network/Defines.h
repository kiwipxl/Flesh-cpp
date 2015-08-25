#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <memory>
#include <functional>

#define BEGIN_NETWORK_NS    namespace network {
#define END_NETWORK_NS      };

#define BEGIN_SOCK_NS       namespace sock {
#define END_SOCK_NS         };

#define BEGIN_MSG_NS        namespace msg {
#define END_MSG_NS          };

BEGIN_NETWORK_NS

    BEGIN_MSG_NS

        class Message;
        typedef std::shared_ptr<Message> MessagePtr;

        typedef std::function<void(Message*)> CallbackFunc;

    END_MSG_NS

    BEGIN_SOCK_NS
        
        class Socket;
        class SocketPoll;

    END_SOCK_NS

END_NETWORK_NS

#endif