#include <Servo.h>
#include <SoftwareSerial.h>
Servo motor;
SoftwareSerial BTSerial(0,1); // RX : 0, TX : 1

//servo motor
int servo_value = 0 ;
int diff = 1;
int servo = 7; // PIN 7

//ultrasonic sensor
int trig = 8;  // PIN 8
int echo = 9;  // PIN 9


void setup() {
  // put your setup code here, to run once:
  motor.attach(servo);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  BTSerial.begin(9600);
  Serial.begin(9600);
  /*while(1){
    if(BTSerial.available())
      break;
      
  }*/
  
  
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
  BTSerial.write("[");
  Serial.write("[");
  BTSerial.println(int(distance));
  Serial.println(int(distance));
  //BTSerial.print(" ");
  BTSerial.println(servo_value);
  Serial.println(servo_value);
  
  delay(500);

}