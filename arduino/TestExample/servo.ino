#include<Servo.h>
Servo servo;

int value = 0 ;
int diff=1;
void setup() {

  servo.attach(7);
  Serial.begin(9600);
}


void loop() {

  value += diff;
  if(value==0 || value==180 ){
    diff*=-1;
  }
  

  servo.write(value);
  //Serial.println(value);
  delay(10);
  
}
