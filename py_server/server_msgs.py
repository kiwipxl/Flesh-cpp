import sys;
import db;
import socket_manage;
import msg;
import client;
import random;
import game;
import _MID;
import callback;

def verify_params(recv_mid, mid, np):
    return recv_mid == mid and np == mid.num_params;

def got_msg(sock, client_obj, byte_data):
    message = msg.extract_message(sock, client_obj, byte_data);
    if (message != None):
        mid = message.mid;
        params = message.params;
        np = len(params);
        message.callback_result = callback.CALLBACK_RESULT_SUCCESS;
        n = 0;
        for i in range(0, len(client_obj.callbacks)):
            cb = client_obj.callbacks[n];
            if (cb.mid == message.mid):
                cb.func(message);
                if (cb.remove_after_call):
                    del client_obj.callbacks[n];
                    n -= 1;
                else:
                    cb.reset_timeout();
            n += 1;

        msg.log(message);
        return;

        if (verify_params(mid, _MID.RECV_CLIENT_REGISTER_USER_PASS, np)):
            print("username: %s, password: %s" % (params[0], params[1]));
            db.add_user_account(params[0], params[1]);

        elif (verify_params(mid, _MID.RELAY_TEST, np)):
            msg.send(sock, client_obj, msg.build(_MID.RELAY_TEST, client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

        elif (verify_params(mid, _MID.SEND_CLIENT_ID, np)):
            pass;

        elif (verify_params(mid, _MID.RECV_CLIENT_BINDED_UDP_PORT, np)):
            client_obj.c_udp_port = params[0];

        elif (verify_params(mid, _MID.RECV_UDP_PEER_BIND_PORT_SUCCESS, np)):
            client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[0], params[1], params[2]);

        elif (verify_params(mid, _MID.RECV_UDP_PEER_BIND_PORT_FAILED, np)):
            client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[0], params[1], -1);

        elif (verify_params(mid, _MID.RECV_PEER_CONNECT_SUCCESS, np)):
            client_obj.joined_game.received_connect_success(client_obj.game_client, params[0], params[1]);

        elif (verify_params(mid, _MID.BEGIN_RELAY_TEST, np)):
            msg.send(sock, client_obj, msg.build(_MID.RELAY_TEST, client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));
    else:
        print("received msg (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));