import sqlite3, json


ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e"
)
def fn():
  log = open("log.tmp", "w")
  log.writelines("{")
  con = sqlite3.connect('429.db')
  for row in ctrl:
    with con:
      cur = con.cursor()
      cur.execute( "SELECT time,data from "+row+" order by time desc limit  1")
      r = cur.fetchone()
    j = json.dumps(r)
    print j
    if row == "WATER_TEMP": 
      x =('"{0}" :"{1} '.format(row, j))
      log.writelines(x)
    else:
      x =(', "{0}" : "{1}"'.format(row, j))
      log.writelines(x)
  log.writelines("}")

while True:
  fn()
