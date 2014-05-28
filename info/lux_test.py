import sqlite3, json

ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e"
)
con = sqlite3.connect('ctrl_data.db')
with con:
  cur = con.cursor()
  cur.execute( "SELECT * from LUX order by rowid desc limit 1")
  column_names = [d[0] for d in cur.description]
  # print the latest of each entry
  for row in cur:
    data = dict(zip(column_names, row))
    out = json.dumps(data)
print out
