import time
import serial
import sqlite3 as sq
#set some basic  flags
light_flag = 1
#set some command values
ctrl = dict( LUX = "a", 
  TEMP = "b",
  PRESSURE = "c",
  PH = "d",
  WATER_TEMP = "e"
)
cmnds = dict(
  LIGHT_ON = "y",
  LIGHT_OFF = "z",
  PUMP_ON = "g",
  PUMP_OFF = "h"
)

class Controller:
  def __init__(self ):
    self.sense_com = serial.Serial('/dev/ttyACM1', '57600', timeout = None)
    self.pump_com = serial.Serial('/dev/ttyACM0', '9600', timeout = None)
    time.sleep(2)
  def poll_sensor(self, cmnd):  #use this method to get sensor data
    self.sense_com.write(cmnd) #send command byte
    self.sense_com.flush()
    time.sleep(0.2)
    return  float(self.sense_com.readline()) #return value as a float
  def ctrl_device  (self, cmnd):  
    #use this method to control lights, pump. heater
    self.sense_com.write(cmnd) #send command byte
    self.sense_com.flush()
  def light_cycle(self, cycle):
    if self.poll_sensor(ctrl['LUX']) > 100:
      light_flag = 1 
    else: 
      light_flag = 0
    ref_time = time.strftime("%H:%M:%S") 
    interval = ref_time.split()
    off_cycle =  24 - cycle #14, for strawberries
    off_start =  19 - off_cycle
    hr = int(interval[0][:2])
    # set the light to turn on at 7 pm
    if  hr >=19 or hr <= off_start: #after 7 pm abd 9 am
      if light_flag != 1: # first time only or there's something wrong
        self.ctrl_device(cmnds['LIGHT_ON'])	
        light_flag = 1  #flag that the light is on... 
        #we can do this with sensor feedback too
      else:
       er = open("error_log", "a+")
       er.writelines("lights conflict with schedule, please check database {0}".format(time.time()))
    else:
      self.ctrl_device(cmnds['LIGHT_OFF'])   
    
cnt = Controller()
class Keeper():
  def __init__(self):
    self.con = sq.connect('/home/breddybest/projects/datalogger/sensors.db')
  def put_data(self, data, sensor):
    with self.con:
      cur = self.con.cursor()
      cur.execute('create table if not exists ' + sensor + ' (time REAL, data REAL) ')
      
      cur.execute("INSERT INTO "+sensor+" VALUES(?,?)", (time.time(), data))
      self.con.commit()
  def collect_data(self):
    for key, cmd in ctrl.items():
      print cnt.poll_sensor(cmd), key
      self.put_data(cnt.poll_sensor(cmd), key)
      cnt.light_cycle(14)	

if __name__ == '__main__':
  while True:
    datamkr = Keeper()
    datamkr.collect_data()
  #datamkr.con.close()
