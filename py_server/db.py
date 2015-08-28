import sqlite3 as lite;
import debug;
import accounts;

con = None;
cur = None;

def init():
    global con;
    global cur;
    con = lite.connect("acc.db");
    con.row_factory = lite.Row;     #fetches use dictionary over tuple
    cur = con.cursor();

    query("create table if not exists accounts(id integer primary key, user character, pass character, \
            gold integer, last_login_time integer, last_logout_time integer, total_login_time integer, unique(user))");
    con.commit();

    debug.log("accounts db initiated", debug.P_INFO);

#modifies a attribute attached to an account (user, pass, gold, ect)
def set_attrib(unique_id, attrib_name, attrib_value):
    global cur;

    result = query("update accounts set %s=? where id=?" % (attrib_name), (attrib_value, unique_id));
    if not (result or cur.rowcount == 0): return accounts.GeneralResult.UNKNOWN_ERROR;

    return accounts.GeneralResult.SUCCESS;

#modifies a attribute attached to an account (user, pass, gold, ect)
def get_attrib(unique_id, attrib_name):
    global cur;

    result = query("select %s from accounts where id=?" % (attrib_name), (unique_id, ));
    if not (result): return accounts.GeneralResult.UNKNOWN_ERROR, None;
    f = cur.fetchone();
    if (f == None): return accounts.GeneralResult.UNKNOWN_ERROR, None;

    return accounts.GeneralResult.SUCCESS, f[attrib_name];

#attempts to add a username and password to the database and returns a RegisterResult enum
def add_user_account(username, password):
    global con;
    global cur;

    username, password, err = accounts.format_user_pass(username, password);
    if (err == False): return accounts.RegisterResult.INVALID_FORMAT;

    query("select * from accounts where user=?", (username, ));
    if (cur.fetchone() != None): return accounts.RegisterResult.USER_ALREADY_EXISTS;

    result = query("insert or ignore into accounts(user, pass, gold, last_login_time, last_logout_time, total_login_time) \
                    values(?, ?, ?, ?, ?, ?)", (username, password, accounts.STARTING_GOLD, 0, 0, 0));
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

    return accounts.LoginResult.SUCCESS, fetch["id"];

#returns a LoginResult enum on whether or not the username and password was found in the db
def get_user_acc_details(acc_details, unique_id):
    global cur;

    result = query("select * from accounts where id=?", (unique_id, ));
    if not (result): return accounts.GeneralResult.UNKNOWN_ERROR, acc_details;
    fetch = cur.fetchone();
    if (fetch == None): return accounts.GeneralResult.ERROR, acc_details;
    
    acc_details.username = str(fetch["user"]);
    acc_details.gold = int(fetch["gold"]);

    return accounts.GeneralResult.SUCCESS, acc_details;

#modifies the gold value for a specific account
def set_acc_gold(unique_id, gold):
    return set_attrib(unique_id, "gold", gold);

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