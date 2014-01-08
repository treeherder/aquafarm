import cntrlr as cnt
import sqlite3
con = sqlite3.connect('sensors.db')
for table in cnt.ctrl:
  with con:
    cur = con.cursor()
    cur.execute( "SELECT time,data from "+ table)
    while 1:
      r = cur.fetchone()
      if r ==None:
        break
      print r

