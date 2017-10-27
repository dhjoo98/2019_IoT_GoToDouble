//26사단 정비대대 일반지원중대 임동희

#include <Servo.h>
#include <SoftwareSerial.h>

int servoPin1 = 8;
int servoPin2 = 9;

SoftwareSerial BTSerial(2, 3);    /// (Rx, Tx)

Servo servo1;
Servo servo2;

int flag, angle1, angle2;

void setup() {
Serial.begin(9600);
BTSerial.begin(9600);
BTSerial.write("AT+NAMEImageTracker");

servo1.attach(servoPin1);
servo2.attach(servoPin2);
}

void loop() {
  if(BTSerial.available()){
    int a= BTSerial.read();
 
  if(a>180){
    flag=a;
  }
    else if(a<=180){
      if(flag==181){
        angle1=a;
        Serial.println(angle1);
        servo1.write(angle1);
        }
      else if(flag==182){
        angle2=a;
        Serial.println(angle2);
        servo2.write(angle2);
        }
    }   
}
}

