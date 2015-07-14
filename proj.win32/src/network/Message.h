#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>
#include <stdarg.h>
#include <sstream>
#include "ReadableType.h"
#include "Socket.h"

enum MID {
	MSG_UNKNOWN,
	MSG_CLIENT_ID,
	MSG_CLIENT_USER_PASS
};

class Message {

	public:
		MID msg_id = MSG_UNKNOWN;
		char* params;
		std::string raw_data;
};

namespace Msg {

class EncodeStream {

	public:
		std::ostringstream stream;

		EncodeStream() { }

		template <class T> EncodeStream &operator<<(const T &v) {
			std::string name = type_to_string(v);
			if (name == "int") {
				stream << int2chrstr((int)v, 4);
			}else if (name.find("char") != -1) {
				stream << v;
			}else {
				stream << "UNDEFINED";
			}
			return *this;
		}
};

#define Encode EncodeStream()

extern std::string int2chrstr(int num, int bits);
extern int chrstr2int(std::string chrstr);

extern Message make(MID msg_id, EncodeStream* params);
extern std::string encode(MID msg_id, EncodeStream* params);

extern void send(Socket* sock, EncodeStream* stream_data);

//
//def decode_msg(msg):
//    raw_data = msg.raw_data;
//    if (len(raw_data) >= 4):
//        msg_id = chrstr2int(raw_data[0:3]);
//        params = [];
//        i = 0;
//        start_index = -1;
//        slen = raw_data.__len__() + 1;
//        index = 4;
//        while index < slen:
//            if (i == 0):
//                if (start_index != -1):
//                    params.append(raw_data[start_index:index]);
//                if (index + 2 < slen):
//                    i = chrstr2int(raw_data[index:index + 2]);
//                    start_index = index + 2;
//                    index += 2;
//            i -= 1;
//            index += 1;
//
//        msg.msg_id = msg_id;
//        msg.params = params;
//        return msg;
//
//    msg.msg_id = ID.UNKNOWN;
//    return msg;
//
//def decode_raw(raw_data):
//    m = Message();
//    m.raw_data = raw_data;
//    return decode_msg(m);
//
//def send(sock, msg_id, *params):
//    sock.send(buffer(encode(msg_id, *params).encode()));
//
//def send_msg(sock, msg):
//    sock.send(buffer(msg.raw_data.encode()));
//
//def send_raw(sock, raw_data):
//    sock.send(buffer(raw_data.encode()));
//
//def broadcast_msg(sock_list, msg):
//    for sock in sock_list:
//        sock.send(buffer(msg.raw_data.encode()));
//
//def broadcast_raw(sock_list, msg_id, *params):
//    for sock in sock_list:
//        sock.send(buffer(encode(msg_id, params).encode()));
//
//def broadcast_raw(sock_list, raw_data):
//    for sock in sock_list:
//        sock.send(buffer(raw_data.encode()));
//
}

#endif