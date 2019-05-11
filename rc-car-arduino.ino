//Digital pin config
const int in1 = 2;       //Motor 1-A
const int in2 = 4;       //Motor 1-B
const int in3 = 7;       //Motor 2-A
const int in4 = 8;       //Motor 2_b
const int pwm1 = 3;      //PWM motor A
const int pwm2 = 9;      //PWM motor B
const int zv = 12;       //Speaker
const int L1 = 0;        //Front LED
const int L2 = 1;        //Rear LED
const int trigPin1 = 5;  //Front trigger
const int trigPin2 = 10; //Rear trigger
const int echoPin1 = 6;  //Front echo
const int echoPin2 = 11; //Rear echo

//Analog pin config
const int o0 = A0;
const int o1 = A1;
const int o2 = A2;
const int o3 = A3;
const int o4 = A4;
const int o5 = A5;

byte an_sens4 = 0;
byte an_sens5 = 0;
//
int code[4];
int bcode[4];
int uk;
int pr_p1[6];
int pr_p2[6];
int pr_k1[4];
int pr_k2[4];
int sum1p, sum1k;
int sum2p, sum2k;
int avr1;
int avr2;
int maximum1, minimum1;
int maximum2, minimum2;
bool loops = false;

long duration1, duration2;
int distance1, distance2;

void setup()
{
   //Initial state setup
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   pinMode(in3, OUTPUT);
   pinMode(in4, OUTPUT);
   pinMode(pwm1, OUTPUT);
   pinMode(pwm2, OUTPUT);
   pinMode(trigPin1, OUTPUT);
   pinMode(trigPin2, OUTPUT);
   pinMode(echoPin1, INPUT);
   pinMode(echoPin2, INPUT);
   pinMode(zv, OUTPUT);
   pinMode(L1, OUTPUT);
   pinMode(L2, OUTPUT);
   pinMode(o0, INPUT);
   pinMode(o1, INPUT);
   pinMode(o2, INPUT);
   pinMode(o3, INPUT);
   pinMode(o4, INPUT);
   pinMode(o5, INPUT);
   digitalWrite(zv, LOW);
}

void loop()
{
   sens_lights();
   decode();
   drive();
}

void sens_lights()
{
   an_sens4 = analogRead(o4);
   if (an_sens4 > 100)
   {
      beep();
   }
   an_sens5 = analogRead(o5);
   if (an_sens5 > 100)
   {
      digitalWrite(L1, HIGH);
      digitalWrite(L2, HIGH);
   }
   else
   {
      digitalWrite(L1, LOW);
      digitalWrite(L2, LOW);
   }
}

void drive()
{
   switch (uk)
   {
   case 0:
      stop();
      break;
   case 1:
      forward();
      break;
   case 2:
      back();
      break;
   case 3:
      left();
      break;
   case 4:
      forw_left();
      break;
   case 5:
      bac_left();
      break;
   case 6:
      right();
      break;
   case 7:
      forw_right();
      break;
   case 8:
      bac_right();
      break;
   }
}

void decode()
{
   code[0] = analogRead(o0);
   code[1] = analogRead(o1);  
   code[2] = analogRead(o2);
   code[3] = analogRead(o3);
   for (byte i = 0; i < 4; i++)
   {
      if (code[i] < 100)
      {
         bcode[i] = 0;
      }
      else
      {
         bcode[i] = 2;
      }
   }
   uk = 0;
   for (int i = 0; i < 4; i++)
   {
      switch (bcode[i])
      {
      case 0:
         break;
      case 2:
         uk += pow_of_2(i);
         break;
      }
   }
}

void beep()
{
   for (int i = 0; i < 6; i++)
   {
      digitalWrite(trigPin1, LOW);
      digitalWrite(trigPin2, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin1, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin1, LOW);
      duration1 = pulseIn(echoPin1, HIGH);
      distance1 = duration1 * 0.034 / 2;
      delayMicroseconds(2);
      digitalWrite(trigPin2, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin2, LOW);
      duration2 = pulseIn(echoPin2, HIGH);
      distance2 = duration2 * 0.034 / 2;
      pr_p1[i] = distance1;
      pr_p2[i] = distance2;
      sum1p += pr_p1[i];
      sum2p += pr_p2[i];
   }
   //Calculating average of readings, discrading min and max value to get better results
   minimum1 = min(min(min(pr_p1[0], pr_p1[1]), (pr_p1[2], pr_p1[3])), (pr_p1[4], pr_p1[5]));
   maximum1 = max(max(max(pr_p1[0], pr_p1[1]), (pr_p1[2], pr_p1[3])), (pr_p1[4], pr_p1[5]));
   minimum2 = min(min(min(pr_p2[0], pr_p2[1]), (pr_p2[2], pr_p2[3])), (pr_p2[4], pr_p2[5]));
   maximum2 = max(max(max(pr_p2[0], pr_p2[1]), (pr_p2[2], pr_p2[3])), (pr_p2[4], pr_p2[5]));
   sum1k = sum1p - minimum1 - maximum1;
   sum2k = sum2p - minimum2 - maximum2;
   avr1 = sum1k / 4;
   avr2 = sum2k / 4;

   if (avr1 < 25 || avr2 < 25)
   {
      if (avr1 <= 3 || avr2 <= 3)
      {
         loops = true;
         if (loops == true)
         {
            digitalWrite(zv, HIGH);
            if (avr1 > 3 && avr2 > 3)
            {
               loops = false;
            }
         }
      }

      else
      {
         if (avr1 <= avr2)
         {
            digitalWrite(zv, HIGH);
            delay(13 * avr1);
            digitalWrite(zv, LOW);
            delay(13 * avr1);
         }
         if (avr1 > avr2)
         {
            digitalWrite(zv, HIGH);
            delay(13 * avr2);
            digitalWrite(zv, LOW);
            delay(13 * avr2);
         }
      }
   }
   sum1p = 0;
   sum2p = 0;
   if (avr1 > 3 && avr2 > 3)
   {
      digitalWrite(zv, LOW);
   }
}

void stop() //0
{
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
}
void forward() //1
{
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   analogWrite(pwm1, 200);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   analogWrite(pwm2, 200);
}

void back() //2
{
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   analogWrite(pwm1, 200);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   analogWrite(pwm2, 200);
}

void left() //3
{
   digitalWrite(in1, LOW);
   digitalWrite(in2, LOW);
   analogWrite(pwm1, 0);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   analogWrite(pwm2, 255);
}

void forw_left() //4
{
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   analogWrite(pwm1, 150);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   analogWrite(pwm2, 255);
}

void bac_left() //5
{
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   analogWrite(pwm1, 150);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   analogWrite(pwm2, 255);
}

void right() //6
{
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   analogWrite(pwm1, 255);
   digitalWrite(in3, LOW);
   digitalWrite(in4, LOW);
   analogWrite(pwm2, 0);
}

void forw_right() //7
{
   digitalWrite(in1, HIGH);
   digitalWrite(in2, LOW);
   analogWrite(pwm1, 255);
   digitalWrite(in3, HIGH);
   digitalWrite(in4, LOW);
   analogWrite(pwm2, 150);
}

void bac_right() //8
{
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
   analogWrite(pwm1, 255);
   digitalWrite(in3, LOW);
   digitalWrite(in4, HIGH);
   analogWrite(pwm2, 150);
}

int pow_of_2(int b)
{
   return 1 << b;
}
