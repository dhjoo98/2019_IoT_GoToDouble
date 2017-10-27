// 김광영 : 아두이노 프로그래밍 및 설계
// 김태윤 : 설계 및 외형디자인

#include <IRremote.h>
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);
const int echoPin = 6;
const int trigPin = 7;
const int MOSE = 8;
const int BUTTON = 9;
int RECV_PIN = 11;
int LED = 13;

int STATE = 0;
int result = 0;
int BUTTONSTATE = 0;
int count = 0;
int temp_value = 0;

long microsecondsToCentimeters(long microseconds)
{return microseconds / 29 / 2;}

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  irrecv.enableIRIn();
  pinMode(LED,OUTPUT);
  pinMode(MOSE, OUTPUT);
  digitalWrite(MOSE,LOW);
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
}

void loop() {
  // Controlling the MOSE
    if(BTSerial.available() > 0)
  { 
    STATE = BTSerial.read(); // Reads the data from the serial port
  }
  BUTTONSTATE = digitalRead(BUTTON);
  if (BUTTONSTATE == LOW)
  {
  BTSerial.write('1');
  }
  else
  {
   BTSerial.write('0');
  }
 
  if (STATE == '1') 
  {
  digitalWrite(MOSE,HIGH);; // MOSE ON
  STATE = 0;
  }
  else if (STATE == '0') 
  {
  digitalWrite(MOSE, LOW); // MOSE ON
  STATE = 0;
  }
  // ------------Ultrasonic Distance
  long duration, cm;
  pinMode(trigPin, OUTPUT); 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT); 
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  //Serial.print(cm);
 // Serial.print("cm"); 
//Serial.println(); 
  delay(50);
    // ------------IR Sensing
  if (irrecv.decode(&results)) {
 //Serial.println("**************************************");
 //Serial.println(results.value);
    result = 4294967295 - results.value;
   // Serial.println(result); // default
    if (result == 0){
      count++;   
      //Serial.println(count);
      delay(50);
    }
    //--------------Distance Control
    if (result != 0 && cm<=20){
        digitalWrite(LED,LOW);
      }
    if (result != 0 && cm>20){
      digitalWrite(LED,HIGH);
      delay(1500);
    }
    if (count == 10) {
      digitalWrite(LED,LOW);
      count = 0;
    }
    irrecv.resume(); // Receive the next value
    delay(100);
  }
  else{  
    digitalWrite(LED,LOW);
  }
  }
