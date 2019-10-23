#include <Servo.h>
#include <SoftwareSerial.h>
Servo motor;
SoftwareSerial BTSerial(2,3); // RX : 2, TX : 3

//servo motor
int servo_value = 0 ;
int diff = 2;
int servo = 7; // PIN 7

//ultrasonic sensor
int trig = 8;  // PIN 8
int echo = 9;  // PIN 9


void setup() {
  // put your setup code here, to run once:
  motor.attach(servo);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  //Serial.begin(9600);
  while(!BTSerial){
    ;
  }
  
  BTSerial.begin(9600);
}

void loop() {
  //Initialize ultrasonic
  //digitalWrite(trig,LOW);
  //digitalWrite(echo,LOW);
  //delayMicroseconds(2);

  servo_value += diff;
  if(servo_value==0 || servo_value==180 ){
    diff*=-1;
  }
  motor.write(servo_value); 
  
  
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  unsigned long duration = pulseIn(echo, HIGH);

  float distance = duration /29.0/2.0;
  int bias = int(distance/20);
  if( bias < 15){
    delay(15-bias);
  }
  
  BTSerial.print(int(distance));
  BTSerial.print(" ");
  BTSerial.println(servo_value);

}
