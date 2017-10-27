//26사단 정비대대 일반지원중대 임동희

#include <SoftwareSerial.h>

const int potentiometerPin1 = 0;
const int potentiometerPin2 = 1;

SoftwareSerial BTSerial(2, 3);    /// (Rx, Tx)

void setup() {
Serial.begin(9600);
BTSerial.begin(9600);
BTSerial.write("AT+NAMEImageSender");
BTSerial.write("AT+ROLE=M");
}

void loop() {
  
int value1 = analogRead(potentiometerPin1);
int value2 = analogRead(potentiometerPin2);

int angle1 = map(value1,368, 1023, 0, 179);  
int angle2 = map(value2,358, 1023, 0, 179);  


int aaa =181;
int bbb =angle1;
Serial.println(aaa,DEC);
 BTSerial.write(aaa);
 Serial.println(bbb,DEC);
 BTSerial.write(bbb);
 int ccc =182;
int ddd =angle2;
Serial.println(ccc,DEC);
 BTSerial.write(ccc);
 Serial.println(ddd,DEC);
 BTSerial.write(ddd);

}
