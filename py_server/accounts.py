import msg;
import client;
import _MID;
import db;

class Enum(set):
    def __getattr__(self, name):
        if name in self:
            return name;
        raise AttributeError;

class LoginResult:
    SUCCESS, INCORRECT_USER_OR_PASS, INVALID_FORMAT, UNKNOWN_ERROR = range(4);

class RegisterResult:
    SUCCESS, USER_ALREADY_EXISTS, INVALID_FORMAT, UNKNOWN_ERROR = range(4);

MIN_USERNAME_LEN = 3;
MAX_USERNAME_LEN = 16;
MIN_PASSWORD_LEN = 3;
MAX_PASSWORD_LEN = 16;

def init_client_account(client_obj):
    client_obj.add_message_handler(_MID.ALL, handle_all_messages);

def handle_all_messages(m):
    if (m.mid == _MID.RECV_CLIENT_ATTEMPT_REGISTER):
        print("attempt register: username: %s, password: %s" % (m.params[0], m.params[1]));
        result = db.add_user_account(m.params[0], m.params[1]);
        msg.send(m.sock, m.client_obj, msg.build(_MID.SEND_CLIENT_ATTEMPT_REGISTER_RESULT, result));

    elif (m.mid == _MID.RECV_CLIENT_ATTEMPT_LOGIN):
        print("attempt login: username: %s, password: %s" % (m.params[0], m.params[1]));
        result = db.find_user_account(m.params[0], m.params[1]);
        msg.send(m.sock, m.client_obj, msg.build(_MID.SEND_CLIENT_ATTEMPT_LOGIN_RESULT, result));