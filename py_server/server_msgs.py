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
    mid = msg.extract_mid(byte_data);
    callback_id = msg.extract_callback(byte_data);
    if (mid != _MID.UNKNOWN):
        (params, err) = msg.extract_params(mid, byte_data);
        if (err != -1):
            n = 0;
            for i in range(0, len(client_obj.callbacks)):
                cb = client_obj.callbacks[n];
                verified = False;
                if (cb.type == callback.UNIQUE_ID):
                    verified = (cb.mid == mid and cb.id == callback_id);
                elif (cb.type == callback.MID or cb.type == callback.MID_LOOP):
                    verified = (cb.mid == mid);
                elif (cb.type == callback.MID_ANY):
                    verified = True;
                if (verified):
                    response_code = cb.func(sock, client_obj, mid, callback_id, params);
                    if (response_code != callback.RESPONSE_NONE):
                        msg.send(sock, client_obj, msg.build((_MID.RESPONSE, callback_id,), response_code));

                    erase = True;
                    if (cb.type == callback.MID or cb.type == callback.MID_ANY):
                        cb.num_callbacks_left -= 1;
                        if (cb.num_callbacks_left > 0): erase = False;
                    elif (cb.type == callback.MID_LOOP):
                        erase = False;
                    if (erase):
                        del client_obj.callbacks[n];
                        --n;

            np = len(params);
            msg.log(client_obj, sock.type, mid, callback_id, params);

            if (verify_params(mid, _MID.RECV_CLIENT_REGISTER_USER_PASS, np)):
                print("username: %s, password: %s" % (params[0], params[1]));
                db.add_user_account(params[0], params[1]);

            elif (verify_params(mid, _MID.RELAY_TEST, np)):
                msg.send(sock, client_obj, msg.build(_MID.RELAY_TEST, client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

            elif (verify_params(mid, _MID.SEND_CLIENT_ID, np)):
                pass;

            elif (verify_params(mid, _MID.RECV_CLIENT_BINDED_UDP_PORT, np)):
                client_obj.c_udp_port = params[0];

            elif (verify_params(mid, _MID.RECV_UDP_SERVER_COMMUNICATION_SUCCESS, np)):
                game.join_game(client_obj);

            elif (verify_params(mid, _MID.RECV_UDP_PEER_BIND_PORT_SUCCESS, np)):
                client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[0], params[1], params[2]);

            elif (verify_params(mid, _MID.RECV_UDP_PEER_BIND_PORT_FAILED, np)):
                client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[0], params[1], -1);

            elif (verify_params(mid, _MID.RECV_PEER_CONNECT_SUCCESS, np)):
                client_obj.joined_game.received_connect_success(client_obj.game_client, params[0], params[1]);

            elif (verify_params(mid, _MID.BEGIN_RELAY_TEST, np)):
                msg.send(sock, client_obj, msg.build(_MID.RELAY_TEST, client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

            elif (verify_params(mid, _MID.UDP_INIT_PING_PONG, np)):
                msg.send_udp(client_obj.udp_sock, client_obj.ip, client_obj.c_udp_port, msg.build(_MID.UDP_INIT_PING_PONG));
    else:
        print("received msg (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));