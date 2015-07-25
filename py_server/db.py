import sqlite3 as lite;
import hashlib;
import debug;

db_con = None;
cur = None;
MAX_USERNAME_LEN = 16;

def init():
    global db_con;
    global cur;
    db_con = lite.connect("acc.db");

    db_con.row_factory = lite.Row;     #fetches use dictionary over tuple
    cur = db_con.cursor();
    query("create table if not exists accounts(id integer primary key, user character, pass character, unique(user))");
    db_con.commit();
    debug.log("accounts db initiated", debug.P_INFO);

def add_user_account(username, password):
    global db_con;
    if (username.__len__() > MAX_USERNAME_LEN): username = username[0:MAX_USERNAME_LEN];
    query("insert or ignore into accounts(user, pass) values('" + username + "', '" + hashlib.sha256(password).hexdigest() + "')");
    db_con.commit();
    debug.log(db_con);

def fetch_accounts():
    global db_con;
    global cur;
    query("select * from accounts");
    rows = cur.fetchall();

    for row in rows:
        debug.log("id: %i, name: %s" % (row["user"], row["pass"]), debug.P_INFO);

def query(q):
    global db_con;
    try:
        db_con.execute(q);
    except lite.Error as e:
        debug.log("database error occurred: %s" % e.args[0], debug.P_ERROR);

if __name__ == "__main__":
    init();