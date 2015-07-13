import sqlite3 as lite
import sys

con = lite.connect("test.db");

with con:
    con.row_factory = lite.Row;
    cur = con.cursor();
    cur.execute("SELECT * FROM users");
    rows = cur.fetchall();

    for row in rows:
        print("id: %i, name: %s" % (row["id"], row["name"]));