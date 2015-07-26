import sys;
import db;
import socket_manage;
import message;
import client;
import time;
import random;
import game;

def verify_params(recv_mid, mid, np):
    return recv_mid == mid and np == mid.num_params;

def got_message(sock, client_obj, byte_data):
    mid = message.extract_mid(byte_data);
    if (mid != message.MID_UNKNOWN):
        (params, err) = message.extract_params(mid, byte_data);
        if (err != -1):
            np = len(params);
            if (verify_params(mid, message.MID_RECV_CLIENT_REGISTER_USER_PASS, np)):
                print("username: %s, password: %s" % (params[0], params[1]));
                db.add_user_account(params[0], params[1]);

            elif (verify_params(mid, message.MID_RELAY_TEST, np)):
                message.print_params(client_obj, sock.type, mid, params);
                message.send(sock, client_obj, message.MID_RELAY_TEST, (client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

            elif (verify_params(mid, message.MID_SEND_CLIENT_ID, np)):
                message.print_params(client_obj, sock.type, mid, params);

            elif (verify_params(mid, message.MID_RECV_CLIENT_BINDED_UDP_PORT, np)):
                message.print_params(client_obj, sock.type, mid, params);
                client_obj.c_udp_port = params[0];

            elif (verify_params(mid, message.MID_RECV_UDP_SERVER_COMMUNICATION_SUCCESS, np)):
                game.join_game(client_obj);

            elif (verify_params(mid, message.MID_RECV_UDP_PEER_BIND_PORT_SUCCESS, np)):
                client_obj.joined_game.received_udp_bind_port(client_obj.game_client, params[2]);

            elif (verify_params(mid, message.MID_RECV_UDP_PEER_BIND_PORT_FAILED, np)):
                client_obj.joined_game.received_udp_bind_port(client_obj.game_client, -1);

            elif (verify_params(mid, message.MID_BEGIN_RELAY_TEST, np)):
                message.send(sock, client_obj, message.MID_RELAY_TEST, (client_obj.id, client_obj.ip, client_obj.c_tcp_port, client_obj.c_udp_port));

            elif (verify_params(mid, message.MID_UDP_INIT_PING_PONG, np) and sock == client_obj.udp_sock):
                message.send_udp(client_obj.udp_sock, client_obj.ip, client_obj.c_udp_port, message.MID_UDP_INIT_PING_PONG);
    else:
        print("received message (raw: %s, len: %d) has an unknown MID" % (byte_data, byte_data.__len__()));