

#include <Wire.h>
#include <math.h> 
#include "MS561101BA.h"
#include "OneWire.h"
OneWire  ds(13);  // on pin 10 (a 4.7K resistor is necessary)
MS561101BA baro = MS561101BA();
byte addr[8] = {0x28, 0x5A, 0xF9, 0x36, 0x04, 0x00, 0x00, 0x75}; //black waterproof sensor

int BH1750address = 0x23; //setting i2c address
int light = 8;
int ph_probe = A0;
int pump = 7;
int heat =6;
byte buff[2];


void setup () {
    Serial.begin(57600);
    pinMode(light, OUTPUT);
    pinMode(pump, OUTPUT);

    Wire.begin();
    // You'll have to check this on your breakout schematics
    baro.init(MS561101BA_ADDR_CSB_LOW);
    digitalWrite(light, LOW); //turn the light on 

}

void loop()
{
  if (Serial.available() > 0){
    int key = Serial.read();
    switch(key){
    case 'a':
      report_lux();
      break;
    case 'b':
      temp();
      break;
    case 'c':
      pres();
      break;
    case 'd':
      water_ph();
      break;
    case 'e':
      water_temp();
      break;/*
    case 'f':
      dh_mtr();
      break;*/
    case'g':
      pump_on();
      break;
    case 'h':
      pump_off();
      break; /*
    case'i':
      heat_on();
      break;
    case 'j'
      heat_off();
      break;    */
    case 'y':
      light_on();
      break;
    case 'z':
      light_off();
      break;
    
    default:
     Serial.println();
     break;
  }
}  
     
}


void report_lux()
{
  int i;
  uint16_t val=0;
  BH1750_Init(BH1750address);
  delay(200);

  if(2==BH1750_Read(BH1750address))
  {
    val=((buff[0]<<8)|buff[1])/1.2;
    Serial.println(val,DEC);     
  }
  delay(150);
}


int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // receive one byte
    i++;
  }
  Wire.endTransmission();  
  return i;
}

int pump_on(){ digitalWrite(pump, LOW); }
int pump_off(){ digitalWrite(pump, HIGH); }
int light_on(){ digitalWrite(light, LOW); }
int light_off(){ digitalWrite(light, HIGH); }


void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
int temp()
{
  float temperature = NULL, pression = NULL;
  while(temperature == NULL) {
    temperature = baro.getTemperature(MS561101BA_OSR_4096);
  }
  Serial.println(temperature);
  
  return 1;
}
int pres()
{
  float temperature = NULL, pression = NULL;
  while(pression == NULL) {
    pression = baro.getPressure(MS561101BA_OSR_4096);
  }
  Serial.println(pression);
  return 1;
}


void water_temp() {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  float celsius, fahrenheit;
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }

  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  celsius = (float)raw / 16.0;
  fahrenheit = celsius * 1.8 + 32.0;
  Serial.println(celsius);
}

void water_ph(){ Serial.println(analogRead(ph_probe));}
