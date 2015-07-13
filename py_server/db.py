import sqlite3 as lite;
import hashlib;

db_con = None;

def init():
    global db_con;
    db_con = lite.connect("acc.db");

    with db_con:
        db_con.row_factory = lite.Row;     #fetches use dictionary over tuple
        cur = db_con.cursor();
        db_con.execute("create table if not exists accounts(id integer primary key, user character(16), pass character(256))");
        print("accounts db initiated");

        add_user("ayy", "pass2");

def add_user(username, password):
    global db_con;
    db_con.execute("insert into accounts(user, pass) values('" + username + "', '" + hashlib.sha256(password).hexdigest() + "')");

def fetch_accounts():
    global db_con;
    cur.execute("select * from accounts");
    rows = cur.fetchall();

    for row in rows:
        print("id: %i, name: %s" % (row["user"], row["pass"]));

if __name__ == "__main__":
    init();