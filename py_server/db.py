import sqlite3 as lite;
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
    query("create table if not exists accounts(id integer primary key, user character, pass character, \
            gold integer, unique(user))");
    con.commit();

    debug.log("accounts db initiated", debug.P_INFO);

#attempts to add a username and password to the database and returns a RegisterResult enum
def add_user_account(username, password):
    global con;
    global cur;

    username, password, err = accounts.format_user_pass(username, password);
    if (err == False): return accounts.RegisterResult.INVALID_FORMAT;

    query("select * from accounts where user='%s'" % (username));
    if (cur.fetchone() != None): return accounts.RegisterResult.USER_ALREADY_EXISTS;

    result = query("insert or ignore into accounts(user, pass, gold) \
                    values('%s', '%s', '%d')" % (username, password, accounts.STARTING_GOLD));
    if not (result): return accounts.RegisterResult.UNKNOWN_ERROR;
    con.commit();

    return accounts.RegisterResult.SUCCESS;

#returns a LoginResult enum on whether or not the username and password was found in the db
def find_user_account(username, password):
    global cur;

    username, password, err = accounts.format_user_pass(username, password);
    if (err == False): return accounts.LoginResult.INVALID_FORMAT, -1;

    result = query("select * from accounts where user='%s' and pass='%s'" % (username, password));
    if not (result): return accounts.LoginResult.UNKNOWN_ERROR, -1;
    fetch = cur.fetchone();
    if (fetch == None): return accounts.LoginResult.INCORRECT_USER_OR_PASS, -1;

    return accounts.LoginResult.SUCCESS, fetch[accounts.ID_INDEX];

#returns a LoginResult enum on whether or not the username and password was found in the db
def get_user_acc_details(acc_details, unique_id):
    global cur;

    result = query("select * from accounts where id='%d'" % (unique_id));
    if not (result): return accounts.GeneralResult.UNKNOWN_ERROR, acc_details;
    fetch = cur.fetchone();
    if (fetch == None): return accounts.GeneralResult.ERROR, acc_details;
    
    acc_details.username = str(fetch[accounts.USER_INDEX]);
    acc_details.gold = int(fetch[accounts.GOLD_INDEX]);

    return accounts.LoginResult.SUCCESS, acc_details;

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