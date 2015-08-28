import msg;
import client;
import _MID;
import db;
import re;
import debug;
import hashlib;
import socket_manage;
import time;

class AccDetails:
    unique_id = -1;
    username = "";
    gold = -1;
    last_login_time = 0;
    last_logout_time = 0;
    total_login_time = 0;
    parent_client = None;

class LoginResult:
    SUCCESS, INCORRECT_USER_OR_PASS, INVALID_FORMAT, UNKNOWN_ERROR = range(4);

class RegisterResult:
    SUCCESS, USER_ALREADY_EXISTS, INVALID_FORMAT, UNKNOWN_ERROR = range(4);

class GeneralResult:
    SUCCESS, ERROR, UNKNOWN_ERROR = range(3);

MIN_USERNAME_LEN = 3;
MAX_USERNAME_LEN = 16;
MIN_PASSWORD_LEN = 3;
MAX_PASSWORD_LEN = 16;

input_regex = "^[a-zA-Z0-9]+$";

STARTING_GOLD = 1225;

def init_client_account(client_obj):
    client_obj.add_message_handler(_MID.ALL, handle_all_messages);

def update_acc_details(client_obj, unique_id):
    result, acc_details = db.get_user_acc_details(client_obj.acc_details, unique_id);
    if (result == GeneralResult.SUCCESS):
        client_obj.acc_details = acc_details;
        debug.log_accounts("updated account details: id: %d, user: %s, gold: %d" %
                           (acc_details.unique_id, acc_details.username, acc_details.gold));

def handle_all_messages(m):
    if (m.mid == _MID.RECV_CLIENT_ATTEMPT_REGISTER):
        result = db.add_user_account(m.params[0], m.params[1]);
        msg.send(m.sock, m.client_obj, msg.build(_MID.SEND_CLIENT_ATTEMPT_REGISTER_RESULT, result));

    elif (m.mid == _MID.RECV_CLIENT_ATTEMPT_LOGIN):
        result, unique_id = db.find_user_account(m.params[0], m.params[1]);
        if (result == LoginResult.SUCCESS):
            acc_details = AccDetails();
            acc_details.parent_client = m.client_obj;
            acc_details.unique_id = unique_id;
            m.client_obj.acc_details = acc_details;
            update_acc_details(m.client_obj, unique_id);

            t = time.time();
            db.set_attrib(unique_id, "last_login_time", t);
            acc_details.last_login_time = t;

            result, total = db.get_attrib(unique_id, "total_login_time");
            if (result == GeneralResult.SUCCESS):
                acc_details.total_login_time = total;

        msg.send(m.sock, m.client_obj, msg.build(_MID.SEND_CLIENT_ATTEMPT_LOGIN_RESULT, result));

    elif (m.mid == _MID.RECV_REQUEST_FOR_CLIENT_ACCOUNT_DETAILS):
        acc_details = m.client_obj.acc_details;
        result = GeneralResult.ERROR;
        username = "";
        gold = -1;
        if (acc_details and acc_details.parent_client == m.client_obj):
            result = GeneralResult.SUCCESS;
            username = acc_details.username;
            gold = acc_details.gold;

        msg.send(m.sock, m.client_obj,
                 msg.build(_MID.SEND_CLIENT_ACCOUNT_DETAILS, result, username, gold));

    elif (m.mid == _MID.RECV_REQUEST_TO_BUY_BOOSTER_PACK):
        acc_details = m.client_obj.acc_details;
        result = GeneralResult.UNKNOWN_ERROR;
        gold = -1;
        if (m.client_obj.acc_details != None):
            cost = 0;
            if (m.params[0] == 0):
                cost = 450;
            elif (m.params[0] == 1):
                cost = 800;
            elif (m.params[0] == 2):
                cost = 2000;
            if (cost != 0):
                gold = acc_details.gold - cost;
                if (gold >= 0):
                    result = db.set_acc_gold(acc_details.unique_id, gold);
                    if (result == GeneralResult.SUCCESS):
                        acc_details.gold = gold;
                else:
                    result = GeneralResult.ERROR;

        msg.send(m.sock, m.client_obj,
                 msg.build(_MID.SEND_REQUEST_TO_BUY_BOOSTER_PACK_RESULT, result, gold));

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

def handle_client_leave(client_obj):
    acc_details = client_obj.acc_details;
    if (acc_details != None):
        t = time.time();
        if (db.set_attrib(acc_details.unique_id, "last_logout_time", t) == GeneralResult.SUCCESS):
            acc_details.last_logout_time = t;

            result, total = db.get_attrib(acc_details.unique_id, "total_login_time");
            if (result == GeneralResult.SUCCESS):
                total += t - acc_details.last_login_time;
                db.set_attrib(acc_details.unique_id, "total_login_time", total);
                acc_details.total_login_time = total;