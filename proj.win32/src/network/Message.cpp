#include "Message.h"
#include "cocos2d.h"

extern std::string Msg::int2chrstr(int num, int bits) {
	std::string s = "";
	if (bits >= 1) s += num % 256;
	if (bits >= 2) s += num / 256;
	if (bits >= 3) s += num / (256 * 256);
	if (bits >= 4) s += num / (256 * 256 * 256);
	return s;
}

extern int Msg::chrstr2int(std::string chrstr) {
	int i = 0;
	int l = chrstr.length();
	if (l >= 4) i += chrstr[3] * (256 * 256 * 256);
	if (l >= 3) i += chrstr[2] * (256 * 256);
	if (l >= 2) i += chrstr[1] * 256;
	if (l >= 1) i += chrstr[0];
	return i;
}

template<class... Args>
extern Message Msg::make(int msg_id, Args... args) {
	Message m = Message();
	/* m.msg_id = msg_id;
	m.params = params;
	m.raw_data = encode(msg_id, *params);*/
	return m;
}

template <class T>
extern std::string Msg::encode(int msg_id, std::initializer_list<T> list) {
	/*std::string data = int2chrstr(msg_id, 4);
	for (param : params) {
	s = str(param);
	if (s != "") : data += int2chrstr(s.__len__(), 2) + s;
	}*/
	std::string = "";
	return data;
}

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