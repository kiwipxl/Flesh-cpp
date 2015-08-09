import msg;
import client;
import _MID;
import db;

def init_client_account(client_obj):
    client_obj.add_message_handler(_MID.ALL, handle_all_messages);

def handle_all_messages(m):
    if (m.mid == _MID.RECV_CLIENT_ATTEMPT_REGISTER):
        print("username: %s, password: %s" % (m.params[0], m.params[1]));
        result = db.add_user_account(m.params[0], m.params[1]);
        msg.send(m.sock, m.client_obj, msg.build(_MID.SEND_CLIENT_ATTEMPT_REGISTER_RESULT, result));

    elif (m.mid == _MID.RECV_CLIENT_ATTEMPT_LOGIN):
        print("username: %s, password: %s" % (m.params[0], m.params[1]));
        result = db.find_user_account(m.params[0], m.params[1]);
        msg.send(m.sock, m.client_obj, msg.build(_MID.SEND_CLIENT_ATTEMPT_LOGIN_RESULT, result));