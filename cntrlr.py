import time
import serial
import sqlite3 as sq

ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e",
	LIGHT_ON = "y",
	LIGHT_OFF = "z")
	
class Controller():
 
  def __init__(self, cmnd):
    self.com = serial.Serial('/dev/ttyACM0', '57600', timeout = None)
    self.cmnd = cmnd
    time.sleep(2)
    
  def poll_sensor (self, cmnd):  #use this method to get sensor data
    self.com.write(self.cmnd) #send command byte
    self.com.flush()
    time.sleep(0.2)
    return  float(self.com.readline()) #return value as a float

 def ctrl_device (self, cmnd):  #use this method to control lights, pump. heater
    self.com.write(self.cmnd) #send command byte
    self.com.flush()
    

class Keeper():
  def __init__(self, sensor, data):
    self.con = sq.connect('sensors.db')
    self.data = data
    self.sensor = sensor
    control = Controller()
  def put_data(self, data, sensor):
    with self.con:
      cur = con.cursor()
      cur.execute('create table if not exists ' + sensor + ' (time, data) ')
      cur.execute("INSERT INTO {0} VALUES({1},{2}".format(sensor, time.time(), self.data))
      self.con.commit()
      self.con.close()
  def collect_data(self):
    for key, cmd in ctrl.items():
      put_data(control.poll_sensor(cmd), key)


datamkr = Keeper()

if __name__ == '__main__':
    datamkr = Keeper()
    datamkr.collect_data()

 
