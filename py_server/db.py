import sqlite3 as lite;
import hashlib;

db_con = None;
cur = None;

def init():
    global db_con;
    global cur;
    db_con = lite.connect("acc.db");

    db_con.row_factory = lite.Row;     #fetches use dictionary over tuple
    cur = db_con.cursor();
    query("create table if not exists accounts(id integer primary key, user character(16), pass character(256), unique(user))");
    db_con.commit();
    print("accounts db initiated");

def add_user_account(username, password):
    global db_con;
    query("insert or ignore into accounts(user, pass) values('" + username + "', '" + hashlib.sha256(password).hexdigest() + "')");
    db_con.commit();
    print(db_con);

def fetch_accounts():
    global db_con;
    global cur;
    query("select * from accounts");
    rows = cur.fetchall();

    for row in rows:
        print("id: %i, name: %s" % (row["user"], row["pass"]));

def query(q):
    global db_con;
    try:
        db_con.execute(q);
    except lite.Error as e:
        print("database error occurred: %s" % e.args[0]);

if __name__ == "__main__":
    init();