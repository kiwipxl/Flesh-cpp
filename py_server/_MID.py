from __future__ import print_function;
import sys;
import _FT;

id = 0;
vec = [];

class MID():

    id = 0;
    ft_params = [];
    total_param_bytes = 0;
    num_params = 0;

    def __init__(self, *ft_params_list):
        global id;
        global vec;

        self.id = id;
        self.ft_params = ft_params_list;
        i = 0;
        for param in ft_params_list:
            self.total_param_bytes += param.len;
            i += 1;
        self.num_params = i;

        id += 1;
        vec.append(self);

#MID syntax note:
#SEND_XXX - a message to send to a client
#RECV_XXX - a message to receive from a client
#XXX - a message that can be both sent and received to and from clients

UNKNOWN                                     = MID();

#sends a client id of a specified client
SEND_CLIENT_ID                              = MID(_FT.INT);
#receives a request to register a username and password from a client
RECV_CLIENT_LOGIN_USER_PASS                 = MID(_FT.CHAR_ARRAY, _FT.CHAR_ARRAY);
#receives a request to register a username and password from a client
RECV_CLIENT_REGISTER_USER_PASS              = MID(_FT.CHAR_ARRAY, _FT.CHAR_ARRAY);

#begins sending a relay test which ping pongs for infinite time
BEGIN_RELAY_TEST                            = MID();
#ping pong relay test messages
RELAY_TEST                                  = MID(_FT.INT, _FT.CHAR_ARRAY, _FT.UNSIGNED_SHORT, _FT.UNSIGNED_SHORT);
#init ping pong udp message to test udp communication
UDP_INIT_PING_PONG                          = MID();
#ping pong udp messages
UDP_PING_PONG                               = MID();

RECV_UDP_SERVER_COMMUNICATION_SUCCESS       = MID();
#requests a client to bind to a random port to allow peers to send messages to
SEND_UDP_PEER_BIND_REQUEST                  = MID(_FT.INT, _FT.CHAR_ARRAY);
#receives the binded udp port from a client after the request was made
RECV_UDP_PEER_BIND_PORT_SUCCESS             = MID(_FT.INT, _FT.CHAR_ARRAY, _FT.UNSIGNED_SHORT);
#the request to bind a udp port on a client failed
RECV_UDP_PEER_BIND_PORT_FAILED              = MID(_FT.INT, _FT.CHAR_ARRAY);
#the request to bind a udp port on a client failed
SEND_UDP_PEER_PORT                          = MID(_FT.INT, _FT.UNSIGNED_SHORT);
#received when a client successfully connect to their peer
RECV_PEER_CONNECT_SUCCESS                   = MID(_FT.INT, _FT.CHAR_ARRAY);

#sent once a peer has joined and successfully communicated with the client
SEND_PEER_JOIN                              = MID(_FT.CHAR_ARRAY, _FT.UNSIGNED_SHORT);
#sent once a client has left the server (tcp, udp connection fail, ect) and needs to tell the client about it
SEND_PEER_LEAVE                             = MID(_FT.CHAR_ARRAY, _FT.UNSIGNED_SHORT);
#received if a client is unable to communicate with other peers
RECV_PEER_LEAVE                             = MID(_FT.CHAR_ARRAY, _FT.UNSIGNED_SHORT);

#server sends server udp binded port to client
SEND_SERVER_BINDED_UDP_PORT                 = MID(_FT.UNSIGNED_SHORT);
#client sends client binded udp port to server
RECV_CLIENT_BINDED_UDP_PORT                 = MID(_FT.UNSIGNED_SHORT);

#put all x variables into a name array so messages can be debugged easier
names = id * [None];
for k, v in list(locals().iteritems()):
    if (k != None and isinstance(v, MID)):
        names[v.id] = k;
