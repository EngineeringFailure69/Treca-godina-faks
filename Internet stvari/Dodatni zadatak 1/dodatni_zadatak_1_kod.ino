#include <Servo.h>
#include <LiquidCrystal.h>

Servo myservo;
LiquidCrystal lcd(4, 5, 6, 7, 8, 10);

//Pinovi
const byte clockwiseButtonPin = 2;
const byte counterClockwiseButtonPin = 3;
const byte ledPin = 11;
const int triggerPin = 12;
const int echoPin = 14;
const int potPin = A0;
int servoPin = 9;

//Pomocne promenljive
int clockwiseButtonValue, counterClockwiseButtonValue;
int pos = 0, stopDistance = 6;
int minmaxAngle = 180;
byte ledState = HIGH;
volatile boolean rotateCW = false, rotateCCW = false;
unsigned long lastMoveTime = 0;
const int moveInterval = 16; //milisekunde
int lineToClear = 0;
int NAPRED = 0, NAZAD = 1, CEKANJE = 2, STOP = 3, PREPREKA = 4;
int trenutnoStanjeEkrana = CEKANJE, prethodnoStanjeEkrana = NAPRED, prethodnoStanjePrePrepreke = PREPREKA;
volatile int counter = 0, fifth_seconds = 0;
float measuredDistance = 0;
boolean cekanje = true;

void setup()
 { 
   cli();
   
   pinMode(clockwiseButtonPin, INPUT);
   pinMode(counterClockwiseButtonPin, INPUT);
   pinMode(ledPin, OUTPUT);
   pinMode(triggerPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(potPin, INPUT);
   myservo.attach(servoPin);
   myservo.write(pos);
   attachInterrupt(digitalPinToInterrupt(clockwiseButtonPin), rotateClockwise, CHANGE);
   attachInterrupt(digitalPinToInterrupt(counterClockwiseButtonPin), rotateCounterClockwise, CHANGE);
   lcd.begin(16, 2);
   
   //CTC mod
   TCCR2A=(1<<WGM21);
   //Preskaler 1/64 takta
   TCCR2B=(1<<CS22); 

   OCR2A=0xF9; //Za 1ms: (16*10^6/64)-1 = 1000/4 = 250 - 1 = 249 (F9 hex) 
  
   TIMSK2|=(1<<OCIE2A);  
   
   sei();
 }

void loop()
{    
   unsigned long currentTime = millis();
   int potValue = analogRead(potPin);
   int step = map(potValue, 0, 1023, 5, 20);
   
   if(fifth_seconds == 1)
   {
      measuredDistance = measureDistance(triggerPin, echoPin);
      fifth_seconds = 0;
      if(measuredDistance < stopDistance)
	 trenutnoStanjeEkrana = PREPREKA;
      else if(pos >= minmaxAngle && !rotateCW && !cekanje)
	 trenutnoStanjeEkrana = STOP;
      else if(pos <= 0 && !rotateCCW && !cekanje)
	 trenutnoStanjeEkrana = STOP; 
      else if(cekanje)
	 trenutnoStanjeEkrana = CEKANJE;
   }
   
   //Smer kazaljke
   if(rotateCW && currentTime - lastMoveTime >= moveInterval && measuredDistance >= stopDistance)
   {
      lastMoveTime = currentTime;
      pos += step;
      trenutnoStanjeEkrana = NAPRED;
      if(pos >= minmaxAngle)
      {
	 pos = minmaxAngle;
	 rotateCW = false;
	 digitalWrite(ledPin, ledState);
	 trenutnoStanjeEkrana = STOP;
	 cekanje = false;
      }
      else
	 digitalWrite(ledPin, !ledState);
      myservo.write(pos);
   }
   
   //Kontra od kazaljke
   else if(rotateCCW && currentTime - lastMoveTime >= moveInterval && measuredDistance >= stopDistance)
   {
      trenutnoStanjeEkrana = NAZAD;
      lastMoveTime = currentTime;
      pos -= step;
      if(pos <= 0)
      {
	 rotateCCW = false; 
	 pos = 0;
	 digitalWrite(ledPin, ledState);
	 trenutnoStanjeEkrana = STOP;
	 cekanje = false;
      }
      else 
	 digitalWrite(ledPin, !ledState);
      myservo.write(pos);
   }
   
   updateDisplay();
}
 
//ISR funkcije 
 
 void rotateClockwise()
 {
   rotateCW = true;
   rotateCCW = false;
 }
 
 void rotateCounterClockwise()
 {
   rotateCW = false;
   rotateCCW = true;
 }
 
ISR(TIMER2_COMPA_vect)
{
   counter++;
   if(counter == 200)
   {
      fifth_seconds++;
      counter = 0;
   }
}
 
 //Pomocne funkcije
 
void updateDisplay()
{
   if(trenutnoStanjeEkrana != prethodnoStanjeEkrana)
   {
      lcd.clear();
      lcd.setCursor(0, 0);
      if(trenutnoStanjeEkrana == NAPRED)
	 lcd.print("NAPRED");
      else if(trenutnoStanjeEkrana == NAZAD)
	 lcd.print("NAZAD");
      else if(trenutnoStanjeEkrana == STOP)
	 lcd.print("STOP");
      else if(trenutnoStanjeEkrana == PREPREKA)
	 lcd.print("PREPREKA");
      else
	 lcd.print("CEKANJE");
   }
   prethodnoStanjeEkrana = trenutnoStanjeEkrana;
}

float measureDistance(int trigger, int echo)
{
   long duration = 0;
   int distanceCm = 0;
   digitalWrite(triggerPin, LOW);
   delayMicroseconds(2);
   digitalWrite(triggerPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(triggerPin, LOW);
   duration = pulseIn(echoPin, HIGH, 30000);
   distanceCm = duration * 0.034 / 2;
   return distanceCm;
}