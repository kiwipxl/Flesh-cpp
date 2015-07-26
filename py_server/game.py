import client;
import message;

class Peer:
    game_client = None;
    binded = False;
    connected = False;

    def __init__(self):
        self.game_client = None;
        self.binded = False;
        self.connected = False;

class GameClient:
    id = None;
    client_obj = None;
    current_game = None;
    peers = [];

    def __init__(self):
        self.id = -1;
        self.client_obj = None;
        self.current_game = None;
        self.peers = [];

class Game:

    g_clients = [];

    def __init__(self):
        self.g_clients = [];

    def client_join(self, client_obj):
        new_gcl = GameClient();
        client_obj.joined_game = self;
        client_obj.game_client = new_gcl;
        new_gcl.id = client_obj.id;
        new_gcl.client_obj = client_obj;
        self.g_clients.append(new_gcl);

        if (len(self.g_clients) >= 2):
            for gcl in self.g_clients:
                if (gcl != new_gcl):
                    #send a bind request to all clients except the newly joined one
                    message.send(gcl.client_obj.tcp_sock, gcl.client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (client_obj.id, client_obj.ip,));
                    #add 1 new peer for the newly joined client
                    joined_peer = Peer();
                    joined_peer.game_client = new_gcl;
                    gcl.peers.append(joined_peer);
                    print("added peer id %d to game client %d. peers len: %d" % (new_gcl.client_obj.id, gcl.client_obj.id, len(gcl.peers)));

                    #send a bind request to the newly joined client for every other client in the game
                    message.send(client_obj.tcp_sock, client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (gcl.client_obj.id, client_obj.ip,));
                    #append the newly joined clients peer list with every other client in the game
                    client_peer = Peer();
                    client_peer.game_client = gcl;
                    new_gcl.peers.append(client_peer);
                    print("2added peer id %d to game client %d. peers len: %d" % (gcl.client_obj.id, new_gcl.client_obj.id, len(new_gcl.peers)));

    def received_udp_bind_port(self, game_client, port):
        if (port >= 0):
            c_id = game_client.client_obj.id;
            c_ip = game_client.client_obj.ip;

            for gcl in self.g_clients:
                for p in gcl.peers:
                    if (p.game_client.client_obj.id == c_id and p.game_client.client_obj.ip == c_ip):
                        p.binded = True;

            print("peer id %d binded" % game_client.client_obj.id);

            all_binded = True;
            for gcl in self.g_clients:
                for p in gcl.peers:
                    if (p.binded == False):
                        all_binded = False;
                        break;

            if (all_binded):
                print("all are binded!");
        else:
            print("udp bind failed, dunno how to handle this right now");

games = [];

def join_game(client_obj):
    if (len(games) == 0):
        g = Game();
        games.append(g);
    else:
        g = games[0];
    g.client_join(client_obj);