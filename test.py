import sqlite3

ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e"
)
con = sqlite3.connect('ctrl_vals.db')
for row in ctrl:
  with con:
    cur = con.cursor()
    cur.execute( "SELECT time,data from "+table+" order by time desc limit 1")
    # print the latest of each entry
    print row  
  while 1:
      r = cur.fetchone()
      if r ==None:
        break
      print r


