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

    query("select * from accounts where user=?", (username, ));
    if (cur.fetchone() != None): return accounts.RegisterResult.USER_ALREADY_EXISTS;

    result = query("insert or ignore into accounts(user, pass, gold) \
                    values(?, ?, ?)", (username, password, accounts.STARTING_GOLD));
    if not (result): return accounts.RegisterResult.UNKNOWN_ERROR;

    return accounts.RegisterResult.SUCCESS;

#returns a LoginResult enum on whether or not the username and password was found in the db
def find_user_account(username, password):
    global cur;

    username, password, err = accounts.format_user_pass(username, password);
    if (err == False): return accounts.LoginResult.INVALID_FORMAT, -1;

    result = query("select * from accounts where user=? and pass=?", (username, password));
    if not (result): return accounts.LoginResult.UNKNOWN_ERROR, -1;
    fetch = cur.fetchone();
    if (fetch == None): return accounts.LoginResult.INCORRECT_USER_OR_PASS, -1;

    return accounts.LoginResult.SUCCESS, fetch[accounts.ID_INDEX];

#returns a LoginResult enum on whether or not the username and password was found in the db
def get_user_acc_details(acc_details, unique_id):
    global cur;

    result = query("select * from accounts where id=?", (unique_id, ));
    if not (result): return accounts.GeneralResult.UNKNOWN_ERROR, acc_details;
    fetch = cur.fetchone();
    if (fetch == None): return accounts.GeneralResult.ERROR, acc_details;
    
    acc_details.username = str(fetch[accounts.USER_INDEX]);
    acc_details.gold = int(fetch[accounts.GOLD_INDEX]);

    return accounts.LoginResult.SUCCESS, acc_details;

#modifies the gold value for a specific account
def set_acc_gold(unique_id, gold):
    global cur;

    result = query("update accounts set gold=? where id=?", (gold, unique_id));
    if not (result or cur.rowcount == 0): return accounts.GeneralResult.UNKNOWN_ERROR;

    return accounts.LoginResult.SUCCESS;

def query(*args):
    global cur;
    global con;

    if (len(args) == 0): return False;

    try:
        cur.execute(*args);
        con.commit();
        return True;
    except lite.Error as err:
        debug.log_db("database error occurred: %s" % err.args[0]);
        return False;

if __name__ == "__main__":
    init();