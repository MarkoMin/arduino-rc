#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = ""; //Insert your unique Blynk auth code, which is generated in your Blynk profile settings
char ssid[] = ""; //Insert your WiFi SSID here, ie: "Markos_WIFI"
char pass[] = ""; //Insert your WiFi password here, ie: "password123"

int Bit0 = 16;      //A0  - bit0
int Bit1 = 5;       //A1  - bit1
int Bit2 = 4;       //A2  - bit2
int Bit3 = 0;       //A3  - bit3
int S = 2;          //A4  - sensors on/off
int L = 14;         //A5  - LED's on/off
int x0, x1, x2, x3; //Virtual pins
byte code = 0;
byte pr0, pr1, pr2, pr3;

void setup()
{
   //Initial state setup
   Blynk.begin(auth, ssid, pass);
   pinMode(Bit0, OUTPUT);
   pinMode(Bit1, OUTPUT);
   pinMode(Bit2, OUTPUT);
   pinMode(Bit3, OUTPUT);
   pinMode(S, OUTPUT);
   pinMode(L, OUTPUT);
   digitalWrite(Bit0, LOW);
   digitalWrite(Bit1, LOW);
   digitalWrite(Bit2, LOW);
   digitalWrite(Bit3, LOW);
   digitalWrite(S, LOW);
   digitalWrite(L, LOW);
}

void loop()
{
   Blynk.run();
   calc();
   get_code();
}

//Calculates code which will be sent to Arduino
void calc()
{
   code = pr0 + pr1 + pr2 + pr3;
   Serial.println(code);
}

//Sends code to Arduino based on current state
void get_code()
{
   switch (code)
   {
   case 0: //Stop state and unwanted states(ie: 3 or more buttons pressed, left & right at the same time)
   case 6:
   case 14:
   case 22:
   case 24:
   case 26:
   case 28:
   case 30:
      digitalWrite(Bit0, LOW);
      digitalWrite(Bit1, LOW);
      digitalWrite(Bit2, LOW);
      digitalWrite(Bit3, LOW);
      break;
   case 2: //FORWARD, 0001B
      digitalWrite(Bit0, HIGH);
      digitalWrite(Bit1, LOW);
      digitalWrite(Bit2, LOW);
      digitalWrite(Bit3, LOW);
      break;
   case 4: //BACK, 0010B
      digitalWrite(Bit0, LOW);
      digitalWrite(Bit1, HIGH);
      digitalWrite(Bit2, LOW);
      digitalWrite(Bit3, LOW);
      break;
   case 8: //LEFT, 0011B
      digitalWrite(Bit0, HIGH);
      digitalWrite(Bit1, HIGH);
      digitalWrite(Bit2, LOW);
      digitalWrite(Bit3, LOW);
      break;
   case 10: //FORWARD-LEFT, 0100B
      digitalWrite(Bit0, LOW);
      digitalWrite(Bit1, LOW);
      digitalWrite(Bit2, HIGH);
      digitalWrite(Bit3, LOW);
      break;
   case 12: //BACK-LEFT, 0101B
      digitalWrite(Bit0, HIGH);
      digitalWrite(Bit1, LOW);
      digitalWrite(Bit2, HIGH);
      digitalWrite(Bit3, LOW);
      break;
   case 16: //RIGHT, 0110B
      digitalWrite(Bit0, LOW);
      digitalWrite(Bit1, HIGH);
      digitalWrite(Bit2, HIGH);
      digitalWrite(Bit3, LOW);
      break;
   case 18: //FORWARD-RIGHT, 0111B
      digitalWrite(Bit0, HIGH);
      digitalWrite(Bit1, HIGH);
      digitalWrite(Bit2, HIGH);
      digitalWrite(Bit3, LOW);
      break;
   case 20: //BACK-RIGHT, 1000B
      digitalWrite(Bit0, LOW);
      digitalWrite(Bit1, LOW);
      digitalWrite(Bit2, LOW);
      digitalWrite(Bit3, HIGH);
      break;
   }
}

//Virtual pin readings
BLYNK_WRITE(V0)
{
   x0 = param[0].asInt();
   x0 == 1 ? pr0 = 2 : pr0 = 0;
}
BLYNK_WRITE(V1)
{
   x1 = param[0].asInt();
   x1 == 1 ? pr1 = 2 : pr1 = 0;
}
BLYNK_WRITE(V2)
{
   x2 = param[0].asInt();
   x2 == 1 ? pr2 = 2 : pr2 = 0;
}
BLYNK_WRITE(V3)
{
   x3 = param[0].asInt();
   x3 == 1 ? pr0 = 2 : pr0 = 0;
}
BLYNK_WRITE(V4)
{
   int xS = param[0].asInt();
   xS == 1 ? digitalWrite(S, HIGH) : digitalWrite(S, LOW);
}
BLYNK_WRITE(V5)
{
   int xL = param[0].asInt();
   xL == 1 ? digitalWrite(L, HIGH) : digitalWrite(L, LOW);
}
