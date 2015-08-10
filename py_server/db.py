import sqlite3 as lite;
import hashlib;
import debug;
import accounts;

con = None;
cur = None;

def init():
    global con;
    global cur;
    con = lite.connect("acc.db");

    #con.row_factory = lite.Row;     #fetches use dictionary over tuple
    cur = con.cursor();
    query("create table if not exists accounts(id integer primary key, user character, pass character, unique(user))");
    con.commit();

    debug.log("accounts db initiated", debug.P_INFO);

#checks the formatting of both the username annd password to make sure it can be put in the db
#returns (formatted username, formatted password, error code (true = success, false = format_error))
def format_user_pass(username, password):
    if (username.__len__() < accounts.MIN_USERNAME_LEN):
        debug.log_db("could not format username (%s) - it is too short" % (username));
        return "", "", False;
    if (password.__len__() < accounts.MIN_PASSWORD_LEN):
        debug.log_db("could not format password (%s) - it is too short" % (password));
        return "", "", False;
    if (username.__len__() > accounts.MAX_USERNAME_LEN):
        debug.log_db("could not format username (%s...) - it is too long" % (username[0:accounts.MAX_USERNAME_LEN]));
        return "", "", False;
    if (password.__len__() > accounts.MAX_PASSWORD_LEN):
        debug.log_db("could not format password (%s...) - it is too long" % (password[0:accounts.MAX_USERNAME_LEN]));
        return "", "", False;
    password = hashlib.sha256(password).hexdigest();

    err = True;
    return username, password, err;

#attempts to add a username and password to the database and returns a RegisterResult enum
def add_user_account(username, password):
    global con;
    global cur;

    username, password, err = format_user_pass(username, password);
    if (err == False): return accounts.RegisterResult.INVALID_FORMAT;

    query("select * from accounts where user='%s'" % (username));
    if (cur.fetchone() != None): return accounts.RegisterResult.USER_ALREADY_EXISTS;

    result = query("insert or ignore into accounts(user, pass) values('%s', '%s')" % (username, password));
    if not (result): return accounts.RegisterResult.UNKNOWN_ERROR;
    con.commit();

    return accounts.RegisterResult.SUCCESS;

#returns a LoginResult enum on whether or not the username and password was found in the db
def find_user_account(username, password):
    global cur;

    username, password, err = format_user_pass(username, password);
    if (err == False): return accounts.LoginResult.INVALID_FORMAT;

    result = query("select * from accounts where user='%s' and pass='%s'" % (username, password));
    if not (result): return accounts.LoginResult.UNKNOWN_ERROR;
    if (cur.fetchone() == None): return accounts.LoginResult.INCORRECT_USER_OR_PASS;

    return accounts.LoginResult.SUCCESS;

def query(q):
    global cur;
    try:
        cur.execute(q);
        debug.log_db("query: %s" % q);
        return True;
    except lite.Error as e:
        debug.log_db("database error occurred: %s" % e.args[0]);
        return False;

if __name__ == "__main__":
    init();