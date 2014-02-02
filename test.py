from cntrlr import ctrl as ctrl
import sqlite3
con = sqlite3.connect('sensors.db')
for table in ctrl:
  with con:
    cur = con.cursor()
    cur.execute( "SELECT time,data from "+table+" order by time,asc limit 1)
    print table  
  while 1:
      r = cur.fetchone()
      if r ==None:
        break
      print r

