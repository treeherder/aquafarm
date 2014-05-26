#include <Wire.h>
#include <math.h>
#include "MS561101BA.h"
#include "OneWire.h"


//analog mux macros
#define s0 2
#define s1 3
#define s2 4

#define e0 5
#define e1 6

#define AnalogPin A1

#define arrayLength 16

int r0=0;
int r1=0;
int r2=0;

int count=0;

int sensorOutput[arrayLength] = { 0 }; // all elements init to 0
//end analog mux macros


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

  //begin analog mux pinmodes and initial values
  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);

  pinMode(e0,OUTPUT);
  pinMode(e1,OUTPUT);

  digitalWrite(s0,LOW);
  digitalWrite(s1,LOW);
  digitalWrite(s2,LOW);
  //end analog mux pinmodes and initial values
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

//begin extra functions for analog mux

void printResults( int * arrayPointer) {
//output will look like [0, 140, 24, 250, ... ,249 ]
  int i;
  Serial.print("[" );

  for(i=0; i < (arrayLength - 1); i++ ){
    Serial.print(*(arrayPointer + i));
    Serial.print(",");
  }

  Serial.print(*(arrayPointer + i));
  Serial.println("]");
}

void readSensors( int* arrayPointer, int enableBit) {
  int sensorReading=0;


  int i;
  for ( i = 0; i <= 7 ; i++ ) {
    r0=( i & 0x01);
    r1=( ( i >>1) & 0x01);
    r2=( ( i >>2) & 0x01);

    /*
      //prints out r0 r1 r2
       Serial.print(" ");
       Serial.print(r0);
       Serial.print(" ");
       Serial.print(r1);
       Serial.print(" ");
       Serial.println(r2);
     */

    digitalWrite(s0,r0);
    digitalWrite(s1,r1);
    digitalWrite(s2,r2);

    if (enableBit == 0) {
      digitalWrite(e1,1);
      digitalWrite(e0,0);
    } else {
      digitalWrite(e0,1);
      digitalWrite(e1,0);
    }

    if (enableBit == 1) {
      sensorReading = *(arrayPointer + i + 8) = analogRead(AnalogPin);
    } else {
      sensorReading = *(arrayPointer + i) = analogRead(AnalogPin);
    }

    /*
      //prints out individual readings
      Serial.print("Reading on ");
      Serial.print(i + enableBit*8);
      Serial.print(" is ");
      Serial.println(sensorReading);
    */

    delay(10);
  }

}
//end extra functions for analog mux

void report_lux()

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
