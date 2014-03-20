#define SOL1 4
#define SOL2 5
#define SOL3 6
#define SOL4 7
#define SOL5 8
#define SOL6 9
#define SOL7 10

char ch;

int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;
int state5 = 0;
int state6 = 0;
int state7 = 0;

void setup( ){
  int i;
  for (i = SOL1 ; i <= SOL7; i++ ) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(115200);
}

void loop() {
  if (Serial.available())
    {
       ch = Serial.read();
       switch (ch)
       {
         case '1': state1 = !state1; digitalWrite(SOL1 , state1); break;
         case '2': state2 = !state2; digitalWrite(SOL2 , state2); break;
         case '3': state3 = !state3; digitalWrite(SOL3 , state3); break;
         case '4': state4 = !state4; digitalWrite(SOL4 , state4); break;
         case '5': state5 = !state5; digitalWrite(SOL5 , state5); break;
         case '6': state6 = !state6; digitalWrite(SOL6 , state6); break;
         case '7': state7 = !state7; digitalWrite(SOL7 , state7); break;
         default: break;
       }
    }
//reads the flow sensor and returns the data in json
  delay(10);
}

