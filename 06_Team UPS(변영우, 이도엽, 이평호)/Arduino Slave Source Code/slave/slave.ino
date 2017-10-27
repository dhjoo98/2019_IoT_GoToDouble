#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 


SoftwareSerial BTSerial(2, 3);
LiquidCrystal_I2C lcd(0x3F,16,2);

int sw = 10;       // 스위치(SW) 핀 설정
int sw2 = 11;       // 스위치(SW2) 핀 설정
int sw3 = 12;       // 스위치(SW3) 핀 설정
int count = 0;
int cntBnt1 = 0;      // Bnt1 클릭 수 카운트
int cntBnt2 = 0;      // Bnt2 클릭 수 카운트
int reading;          // SW 상태
int previous = LOW;   // SW 이전 상태
int reading2;          // SW2 상태
int previous2 = LOW;   // SW2 이전 상태
int reading3;          // SW3 상태
int previous3 = LOW;   // SW3 이전 상태
int chk = 0;

 
long time = 0;        // LED가 ON/OFF 토글된 마지막 시간
long debounce = 50;  // Debounce 타임 설정

void setup() {
  Serial.begin(9600);
  pinMode(sw, INPUT_PULLUP); // SW 를 설정, 아두이노 풀업저항 사용
  pinMode(sw2, INPUT_PULLUP); // SW2 를 설정, 아두이노 풀업저항 사용
  pinMode(sw3, INPUT_PULLUP); // SW3 를 설정, 아두이노 풀업저항 사용
  BTSerial.begin(9600); // set the data rate for the BT port
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Push Edit Button!");
}


void loop() {
    char data;
    int reading = digitalRead(sw); // SW 상태 읽음
    if(digitalRead(sw) == LOW && cntBnt1 == 0) {
      chk = 1;// SW 초기 상태 무시
      lcd.clear();
    }
    //int reading2 = digitalRead(sw2);  // SW2 상태 읽음
    //int reading3 = digitalRead(sw3);  // SW3 상태 읽음
    int i = 0;
    //reading = digitalRead(sw); // SW 상태 읽음
  //SW 가 눌려졌고 스위치 토글 눌림 경과시간이 Debounce 시간보다 크면 실행
  if (chk == 1 && reading == HIGH && previous == LOW && millis() - time > debounce) {
    if(cntBnt1 < 4)
      cntBnt1++;
    else
      cntBnt1 = 1;
    switch(cntBnt1)
    {
      case 1: // 계급 변경
        BTSerial.write('a');
        Serial.print('1');
        UpDown(1);        
        break;
      case 2: // 군번 변경
        BTSerial.write('a');
        Serial.print('2');
        UpDown(2);        
        break;
      case 3: // 이름 변경
        BTSerial.write('a');
        Serial.print(3);
        UpDown(3);        
        break;
      case 4: // 완료
        BTSerial.write('a');
        Serial.print("4");
        lcd.noBlink();
        //UpDown(4);       
        break;
    }

    time = millis();
  }
  if(BTSerial.available()) {
         data = BTSerial.read();
         BTSerial.flush();
         Serial.print(data);
         lcd.print(data);
   }
   
  previous = reading;
  //previous2 = reading2;
  //previous3 = reading3;

}
void UpDown(int num)
{
  char data;
  while(1){
    int reading = digitalRead(sw);
    int reading2 = digitalRead(sw2);  // SW2 상태 읽음
    int reading3 = digitalRead(sw3);  // SW3 상태 읽음
    
    if (reading2 == HIGH && previous2 == LOW && millis() - time > debounce) {
      if(cntBnt2 < 4)
        cntBnt2++;
      else
        cntBnt2 = 1;
        
        time = millis();
        BTSerial.write(cntBnt2);
       switch(num){
        case 1:
          for(int i = 0; i < 4; i++) {
            lcd.setCursor(i,0);
            lcd.write(32);
          }       
          lcd.setCursor(0,0);
          break;
        case 2:
          for(int i = 4; i < 16; i++) {
            lcd.setCursor(i,0);
            lcd.write(32);
          }
          lcd.setCursor(4,0);
          break;
        case 3:
          for(int i = 0; i < 16; i++) {
            lcd.setCursor(i,1);
            lcd.write(32);
          }
          lcd.setCursor(0,1);
          break;
      }
      lcd.blink();
    }
    if (reading3 == HIGH && previous3 == LOW && millis() - time > debounce) {
      if(cntBnt2 > 1)
        cntBnt2--;
      else
        cntBnt2 = 4;

        time = millis();
        BTSerial.write(cntBnt2);
      switch(num){
        case 1:
          for(int i = 0; i < 4; i++) {
            lcd.setCursor(i,0);
            lcd.write(32);
          }       
          lcd.setCursor(0,0);
          break;
        case 2:
          for(int i = 4; i < 16; i++) {
            lcd.setCursor(i,0);
            lcd.write(32);
          }
          lcd.setCursor(4,0);
          break;
        case 3:
          for(int i = 0; i < 16; i++) {
            lcd.setCursor(i,1);
            lcd.write(32);
          }
          lcd.setCursor(0,1);
          break;
      }
      lcd.blink();
    }
  
  if(BTSerial.available()) {
         data = BTSerial.read();
         BTSerial.flush();
         Serial.print(data);
         lcd.print(data);
   }
    previous2 = reading2;
    previous3 = reading3;
    previous = reading;
    if(reading == LOW && previous == LOW && millis() - time > debounce){
      time = millis();
      break;
    }
   
   }
}


