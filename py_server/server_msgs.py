import sys;
import db;
import socket_manage;
import message;
import client;
import time;
import random;
import game;
import _MID;

def verify_params(recv_mid, mid, np):
    return recv_mid == mid and np == mid.num_params;

def got_message(sock, client_obj, byte_data):
    mid = message.extract_mid(byte_data);
    if (mid != _MID.UNKNOWN):
        (params, err) = message.extract_params(mid, byte_data);
        if (err != -1):
            np = len(params);

            if (verify_params(mid, _MID.RECV_CLIENT_REGISTER_USER_PASS, np)):
                print("username: %s, password: %s" % (params[0], params[1]));
                db.add_user_account(params[0], params[1]);

            elif (verify_params(mid, _MID.RELAY_TEST, np)):
                message.log(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.build(_MID.RELAY_TEST, client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

            elif (verify_params(mid, _MID.SEND_CLIENT_ID, np)):
                message.log(client_obj, sock.type, message.build(mid, params));

            elif (verify_params(mid, _MID.RECV_CLIENT_BINDED_UDP_PORT, np)):
                message.log(client_obj, sock.type, message.build(mid, params));
                client_obj.c_udp_port = params[0];

            elif (verify_params(mid, _MID.RECV_UDP_SERVER_COMMUNICATION_SUCCESS, np)):
                game.join_game(client_obj);

            elif (verify_params(mid, _MID.RECV_UDP_PEER_BIND_PORT_SUCCESS, np)):
                message.log(client_obj, sock.type, message.build(mid, params));
                client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[0], params[1], params[2]);

            elif (verify_params(mid, _MID.RECV_UDP_PEER_BIND_PORT_FAILED, np)):
                message.log(client_obj, sock.type, mid, params);
                client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[0], params[1], -1);

            elif (verify_params(mid, _MID.RECV_PEER_CONNECT_SUCCESS, np)):
                message.log(client_obj, sock.type, mid, params);
                client_obj.joined_game.received_connect_success(client_obj.game_client, params[0], params[1]);

            elif (verify_params(mid, _MID.BEGIN_RELAY_TEST, np)):
                message.send(sock, client_obj, message.build(_MID.RELAY_TEST, client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

            elif (verify_params(mid, _MID.UDP_INIT_PING_PONG, np)):
                message.log(client_obj, sock.type, mid, params);
                message.send_udp(client_obj.udp_sock, client_obj.ip, client_obj.c_udp_port, message.build(_MID.UDP_INIT_PING_PONG));
    else:
        print("received message (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));