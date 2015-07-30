import client;
import message;

class Peer:
    game_client = None;
    binded = False;
    connected = False;
    port = 0;

    def __init__(self):
        self.game_client = None;
        self.binded = False;
        self.connected = False;
        self.port = 0;

class GameClient:
    client_obj = None;
    current_game = None;
    peers = [];

    def __init__(self):
        self.client_obj = None;
        self.current_game = None;
        self.peers = [];

class Game:

    g_clients = [];

    def __init__(self):
        self.g_clients = [];

    def client_join(self, client_obj):
        new_gcl = GameClient();
        new_gcl.client_obj = client_obj;
        self.g_clients.append(new_gcl);

        client_obj.joined_game = self;
        client_obj.game_client = new_gcl;

        #self_peer = Peer();
        #self_peer.game_client = new_gcl;
        #new_gcl.peers.append(self_peer);

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
                    message.send(client_obj.tcp_sock, client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (gcl.client_obj.id, gcl.client_obj.ip,));
                    #append the newly joined clients peer list with every other client in the game
                    client_peer = Peer();
                    client_peer.game_client = gcl;
                    new_gcl.peers.append(client_peer);
                    print("2added peer id %d to game client %d. peers len: %d" % (gcl.client_obj.id, new_gcl.client_obj.id, len(new_gcl.peers)));

    def client_leave(self, client_obj):
        for gcl in self.g_clients:
            for p in gcl.peers:
                if (p.game_client.client_obj == client_obj):
                    gcl.peers.remove(p);
            if (gcl.client_obj == client_obj):
                self.g_clients.remove(gcl);

    def received_udp_bind_port(self, game_client, peer_id, peer_ip, port):
        if (port >= 0):
            for p in game_client.peers:
                if (p.game_client.client_obj.id == peer_id and p.game_client.client_obj.ip == peer_ip):
                    p.binded = True;
                    p.port = port;

            print("peer id %d binded" % game_client.client_obj.id);

            all_binded = True;
            for gcl in self.g_clients:
                for p in gcl.peers:
                    if (p.binded == False):
                        all_binded = False;
                        break;

            if (all_binded):
                print("all binded, sending ping pong requests");
                for gcl in self.g_clients:
                    for p in gcl.peers:
                        pcl = p.game_client.client_obj;
                        message.send(pcl.tcp_sock, pcl, message.MID_SEND_UDP_PEER_PORT, (gcl.client_obj.id, p.port));
        else:
            print("udp bind failed, dunno how to handle this right now");

    def received_connect_success(self, game_client, peer_id, peer_ip):
        for p in game_client.peers:
            if (p.game_client.client_obj.id == peer_id and p.game_client.client_obj.ip == peer_ip):
                p.connected = True;

        print("successful communication with peer %d" % peer_id);

        all_connected = True;
        for gcl in self.g_clients:
            for p in gcl.peers:
                if (p.connected == False):
                    all_connected = False;
                    break;

        if (all_connected):
            print("all connected!");

games = [];

def join_game(client_obj):
    global games;
    if (len(games) == 0):
        g = Game();
        games.append(g);
    else:
        g = games[0];
    g.client_join(client_obj);

def client_leave(client_obj):
    if (client_obj.joined_game):
        client_obj.joined_game.client_leave(client_obj);
