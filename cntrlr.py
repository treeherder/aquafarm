import time
import serial
import sqlite3 as sq

ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e")
	
class Controller:
  def __init__(self ):
    self.com = serial.Serial('/dev/ttyACM0', '57600', timeout = None)
    time.sleep(2)
  def poll_sensor(self, cmnd):  #use this method to get sensor data
    self.com.write(cmnd) #send command byte
    self.com.flush()
    time.sleep(0.2)
    return  float(self.com.readline()) #return value as a float
  def ctrl_device  (cmnd):  
    #use this method to control lights, pump. heater
    self.com.write(cmnd) #send command byte
    self.com.flush()
cnt = Controller()

class Keeper():
  def __init__(self):
    self.con = sq.connect('sensors.db')
  def put_data(self, data, sensor):
    with self.con:
      cur = self.con.cursor()
      cur.execute('create table if not exists ' + sensor + ' (time REAL, data REAL) ')
      cur.execute("INSERT INTO "+sensor+" VALUES(?,?)", (time.time(), data))
      self.con.commit()
  def collect_data(self):
    for key, cmd in ctrl.items():
      self.put_data(cnt.poll_sensor(cmd), key)



if __name__ == '__main__':
    datamkr = Keeper()
    datamkr.collect_data()

 
