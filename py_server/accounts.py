import msg;
import client;
import _MID;
import db;
import re;
import debug;
import hashlib;

class LoginResult:
    SUCCESS, INCORRECT_USER_OR_PASS, INVALID_FORMAT, UNKNOWN_ERROR = range(4);

class RegisterResult:
    SUCCESS, USER_ALREADY_EXISTS, INVALID_FORMAT, UNKNOWN_ERROR = range(4);

MIN_USERNAME_LEN = 3;
MAX_USERNAME_LEN = 16;
MIN_PASSWORD_LEN = 3;
MAX_PASSWORD_LEN = 16;

input_regex = "^[a-zA-Z0-9]+$";

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

#checks the formatting of both the username annd password to make sure it can be put in the db
#returns (formatted username, formatted password, error code (true = success, false = format_error))
def format_user_pass(username, password):
    if (username.__len__() < MIN_USERNAME_LEN):
        debug.log_accounts("could not format username (%s) - it is too short" % (username));
        return "", "", False;
    elif (password.__len__() < MIN_PASSWORD_LEN):
        debug.log_accounts("could not format password (%s) - it is too short" % (password));
        return "", "", False;
    elif (username.__len__() > MAX_USERNAME_LEN):
        debug.log_accounts("could not format username (%s...) - it is too long" % (username[0:accounts.MAX_USERNAME_LEN]));
        return "", "", False;
    elif (password.__len__() > MAX_PASSWORD_LEN):
        debug.log_accounts("could not format password (%s...) - it is too long" % (password[0:accounts.MAX_USERNAME_LEN]));
        return "", "", False;
    elif not (re.search(input_regex, username)):
        debug.log_accounts("could not format username - contains invalid characters");
        return "", "", False;
    elif not (re.search(input_regex, password)):
        debug.log_accounts("could not format password - contains invalid characters");
        return "", "", False;

    password = hashlib.sha256(password).hexdigest();

    err = True;
    return username, password, err;