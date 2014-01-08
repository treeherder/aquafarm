import time
import serial
import sqlite3 as sq
class ctrl:
  LUX = "a"
  TEMP = "b"
  PRESSURE = "c"
	LIGHT_ON = "y"
	LIGHT_OFF = "z"
	
class Controller():
 
  def __init__(self):
    self.com = serial.Serial('/dev/ttyACM0', '57600', timeout = None)
    time.sleep(2)
 
  def get_lux(self):
    self.com.write(ctrl.LUX) # command for ping data
    self.com.flush()
    time.sleep(0.2)
    return  float(self.com.readline())
 
 
  def get_temp(self):
    self.com.write(ctrl.TEMP)  # command for compass data
    time.sleep(0.2)
    return  float(self.com.readline())

  def get_press(self):
    self.com.write(ctrl.PRESSURE)  # command for compass data
    time.sleep(0.2)
    return  float(self.com.readline())



class Keeper():
  def __init__(self, data):
    self.con = sq.connect('sensors.db')
    self.data = data
  def put_data(x):
    with self.con:
      cur = con.cursor()
      cur.execute("{0},{1}".format( time.time(), self.data))