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

#define BEGIN_SERVER_NS     namespace server {
#define END_SERVER_NS       };

typedef unsigned short u_short;
typedef unsigned int u_int;

struct pollfd;

BEGIN_NETWORK_NS

    BEGIN_MSG_NS
        
        class Message;
        typedef std::shared_ptr<Message> MessagePtr;
        
        class Param;
        struct MID;
        struct FormatType;
        enum MID_enum;
        enum GeneralResult;

        typedef const FormatType* CFTYPE;
        typedef const MID const* CMID;

        class MIDCallback;
        enum CallbackResult;
        typedef std::function<void(Message*)> CallbackFunc;
        typedef std::shared_ptr<MIDCallback> CallbackPtr;

        class Stream;

    END_MSG_NS

    BEGIN_SOCK_NS
        
        class Socket;
        class SocketPoll;

    END_SOCK_NS

    BEGIN_SERVER_NS

        enum ServerConnectError;
        typedef std::function<void(int, std::string)> ServerConnectCallback;

    END_SERVER_NS

END_NETWORK_NS

#endif