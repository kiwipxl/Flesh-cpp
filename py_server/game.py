import client;
import message;

class Peer:
    client_obj = None;
    connected = False;

class GameClient:
    client_obj = None;
    current_game = None;
    peer_obj = None;
    peers = [];

class Game:

    g_clients = [];

    def client_join(self, client_obj):
        new_gcl = GameClient();
        client_obj.joined_game = self;
        new_gcl.client_obj = client_obj;
        self.g_clients.append(new_gcl);

        p = Peer();
        new_gcl.peer_obj = p;

        if (len(self.g_clients) >= 2):
            #send a bind request to all clients already in the game
            for gcl in self.g_clients:
                if (gcl != new_glc):
                    message.send(gcl.client_obj.tcp_sock, gcl.client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (client_obj.id,));
                    gcl.peers.append(new_gcl);

            #send a bind request to the newly joined client for every client already in the game
            for gcl in self.g_clients:
                if (gcl != new_glc):
                    message.send(client_obj.tcp_sock, client_obj, message.MID_SEND_UDP_PEER_BIND_REQUEST, (gcl.client_obj.id,));
                    new_glc.peers.append(gcl);
games = [];

def join_game(client_obj):
    if (len(games) == 0):
        g = Game();
        games.append(g);
    else:
        g = games[0];
    g.client_join(client_obj);