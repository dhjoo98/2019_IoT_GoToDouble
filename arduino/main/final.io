#include <SoftwareServo.h>
#include <SoftwareSerial.h>
SoftwareServo motor;
SoftwareSerial BTSerial(2,3); // RX : 2, TX : 3

//servo motor
int servo_value = 0 ;
int diff = 1;   //degree difference
int servo = 7; // PIN 7

//ultrasonic sensor
int trig = 8;  // PIN 8
int echo = 9;  // PIN 9

//fixed length
int spare = 0;
int i = 0, j = 0, mod = 0;
char fixed[11] = "[0000 000]";

void setup() {
  //Initialize
  motor.attach(servo);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  BTSerial.begin(9600);
  /*while(1){
    if(BTSerial.available()){ // wait start signal from Rasberry Pi
      break;              
    }
  }*/
  
  Serial.begin(9600);
}

void loop() {
  //Initialize ultrasonic
  //digitalWrite(trig,LOW);
  //digitalWrite(echo,LOW);
  //delayMicroseconds(2);

  servo_value += diff;
  if(servo_value==0 || servo_value==180 ){    // 0~180
    diff*=-1;
  }
  motor.write(servo_value);
  
  //start signal
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  unsigned long duration = pulseIn(echo, HIGH);

  float distance = duration /29.0/2.0;
  int bias = int(distance/20);  //reduce delay when distance increase
  if( bias < 20){
    delay(20-bias);
  }
  BTSerial.write("!");
  //Serial.write("!");
  BTSerial.print(int(distance));
  BTSerial.write(" ");
  //Serial.print(int(distance));
  //Serial.write(" ");
  BTSerial.print(servo_value);
  //Serial.print(servo_value);
  BTSerial.write("#");
  //Serial.write("#");
  BTSerial.println();
  //Serial.println();
  
  SoftwareServo::refresh();
}