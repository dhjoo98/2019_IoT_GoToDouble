#include <SoftwareSerial.h>
 
#define BT_RXD 8 //To HC06 TXD
#define BT_TXD 7 //To HC06 RXD
SoftwareSerial bluetooth(BT_RXD, BT_TXD); //해당 장치로 시리얼 아웃풋 출력. 
 
void setup(){
  int i;
  int tag;
  //int test_number_1;
  //int test_number_2;
  Serial.begin(9600);
  bluetooth.begin(9600);
}
 
void loop(){
    //Read Sensor here.
    for (int i = 10; i< 2000; i++){
      int tag = 2000-i;
      bluetooth.write("[");
      Serial.println(i);
      bluetooth.println(i);
      bluetooth.println(tag);
      delay(1000);
    }
    delay(2000);
}