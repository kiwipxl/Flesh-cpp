import client;
import message;

class Peer:
    game_client = None;
    binded = False;
    connected = False;

class GameClient:
    client_obj = None;
    current_game = None;
    peers = [];

class Game:

    g_clients = [];

    def client_join(self, client_obj):
        new_gcl = GameClient();
        client_obj.joined_game = self;
        client_obj.game_client = new_gcl;
        new_gcl.client_obj = client_obj;
        self.g_clients.append(new_gcl);

        if (len(self.g_clients) >= 2):
            for gcl in self.g_clients:
                if (gcl != new_gcl):
                    #send a bind request to all clients except the newly joined one
                    message.send(gcl.client_obj.tcp_sock, gcl.client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (client_obj.id,));
                    #add 1 new peer for the newly joined client
                    joined_peer = Peer();
                    joined_peer.game_client = new_gcl;
                    gcl.peers.append(joined_peer);

                    #send a bind request to the newly joined client for every other client in the game
                    message.send(client_obj.tcp_sock, client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (gcl.client_obj.id,));
                    #append the newly joined clients peer list with every other client in the game
                    client_peer = Peer();
                    client_peer.game_client = gcl;
                    new_gcl.peers.append(client_peer);

    def received_udp_bind_port(self, game_client, peer_id, port):
        if (port >= 0):
            for p in game_client.peers:
                if (p.game_client.client_obj.id == peer_id):
                    p.binded = True;

            game_client.peer_obj.binded = True;
            print("game client %d binded" % game_client.client_obj.id);
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