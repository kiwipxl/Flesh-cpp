import sqlite3 as lite;
import hashlib;
import debug;
import accounts;

con = None;
cur = None;
MAX_USERNAME_LEN = 16;
MAX_PASSWORD_LEN = 16;

def init():
    global con;
    global cur;
    con = lite.connect("acc.db");

    #con.row_factory = lite.Row;     #fetches use dictionary over tuple
    cur = con.cursor();
    query("create table if not exists accounts(id integer primary key, user character, pass character, unique(user))");
    con.commit();

    debug.log("accounts db initiated", debug.P_INFO);

def account_strings_to_db(username, password):
    if (username.__len__() > MAX_USERNAME_LEN): username = username[0:MAX_USERNAME_LEN];
    if (password.__len__() > MAX_PASSWORD_LEN): password = password[0:MAX_PASSWORD_LEN];
    password = hashlib.sha256(password).hexdigest();

    err = 0;
    return username, password, err;

#attempts to add a username and password to the database and returns a RegisterResult enum
def add_user_account(username, password):
    global con;
    global cur;

    username, password, err = account_strings_to_db(username, password);
    if (err == -1): return accounts.RegisterResult.INVALID_FORMAT;

    query("select * from accounts where user='%s'" % (username));
    if (cur.fetchone() != None): return accounts.RegisterResult.USER_ALREADY_EXISTS;

    result = query("insert or ignore into accounts(user, pass) values('%s', '%s')" % (username, password));
    if not (result): return accounts.RegisterResult.UNKNOWN_ERROR;
    con.commit();

    return accounts.RegisterResult.SUCCESS;

#returns a LoginResult enum on whether or not the username and password was found in the db
def find_user_account(username, password):
    global cur;

    username, password, err = account_strings_to_db(username, password);
    if (err == -1): return accounts.LoginResult.INVALID_FORMAT;

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