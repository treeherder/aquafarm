import sqlite3, json

ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e"
)
def main():
  log = open("log.tmp", "a+")
  log.writelines("{")
  con = sqlite3.connect('ctrl_vals.db')
  for table in ctrl:
    with con:
      cur = con.cursor()
      cur.execute( "SELECT time,data from "+table+" order by time desc limit 1")
      # print the latest of each entry
      print table 
    while 1:
      r = cur.fetchone()
      if r ==None:
          break
      else:
        j_o =  json.dumps(r)
        print j_o
        if table == "WATER_TEMP": 
          x =('"{0}" :"{1} '.format(table, j))
          log.writelines(x)
        else:
          x =(', "{0}" : "{1}"'.format(table, j))
          log.writelines(x)
  log.writelines("}")

