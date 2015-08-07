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
    name = "";

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

UNKNOWN                                                             = MID();
ALL                                                                 = MID();

#sends a client id of a specified client
SEND_CLIENT_ID                                                      = MID(_FT.INT);
#receives a request to register a username and password from a client
RECV_CLIENT_LOGIN_USER_PASS                                         = MID(_FT.STRING, _FT.STRING);
#receives a request to register a username and password from a client
RECV_CLIENT_REGISTER_USER_PASS                                      = MID(_FT.STRING, _FT.STRING);

#begins sending a relay test which ping pongs for infinite time
SEND_SERVER_CONNECTION_ESTABLISHED_SUCCESSFULLY                     = MID();
#init ping pong udp message to test udp communication
SEND_UDP_PING                                                       = MID();
#init ping pong udp message to test udp communication
RECV_UDP_PONG                                                       = MID();

#received if a client is unable to communicate with other peers
SEND_CLIENT_LEAVE                                                   = MID(_FT.STRING);

#server sends server udp binded port to client
REQUEST_CLIENT_TO_BIND_UDP_PORT                                     = MID(_FT.UNSIGNED_SHORT);
#client sends client binded udp port to server
RECV_CLIENT_BINDED_UDP_PORT                                         = MID(_FT.UNSIGNED_SHORT);

#put all x variables into a name array so messages can be debugged easier
names = id * [None];
for k, v in list(locals().iteritems()):
    if (k != None and isinstance(v, MID)):
        names[v.id] = k;
        v.name = k;
