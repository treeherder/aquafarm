import sqlite3, json

ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e"
)
con = sqlite3.connect('ctrl_vals.db')
out = {}
f = open("/var/www/curtis/dummy-db.csv", 'a+')
with con:
  cur = con.cursor()
  for table in ctrl:
    cur.execute("select * from "+table+" order by rowid desc limit 1")
    column_names = [d[0] for d in cur.description]
  # print the latest of each entry
    for row in cur:
      data = dict(zip(column_names, row))
    out[table] = json.dumps(data)
print out
f.writelines(out)
