//각 아두이노에 업로드 
#include <SoftwareSerial.h>
 
#define BT_RXD 8 //To HC06 TXD
#define BT_TXD 7 //To HC06 RXD
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
 
void setup(){
  int test_number; 
  Serial.begin(9600);
  bluetooth.begin(9600);
}
 
void loop(){
    //Read Sensor here.
    int test_number = random(9);
    Serial.println(test_number);
    bluetooth.println(test_number);
    delay(2000);
}