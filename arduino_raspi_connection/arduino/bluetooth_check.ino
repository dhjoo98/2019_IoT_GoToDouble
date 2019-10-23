//회로 연결을 체크하는 함수.
//업로드 후 시리얼 창에서 "line ending 없음" 설정,
//이후 AT 입력. OK를 리턴하면 성공. 
#include <SoftwareSerial.h>
 
#define BT_RXD 8 //To HC06 TXD
#define BT_TXD 7 //To HC06 RXD
SoftwareSerial bluetooth(BT_RXD, BT_TXD);
 
void setup(){
  Serial.begin(9600);
  Serial.println("ENTER AT Commands:");
  bluetooth.begin(9600);
}
 
void loop(){
  //Write data from HC06 to Serial Monior
  if (bluetooth.available()) {
    Serial.write(bluetooth.read());
  }
  //Write from Serial Monitor to HC06
  if (Serial.available()) {
    bluetooth.write(Serial.read());
  }
}