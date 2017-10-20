#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);


int flame = A0;                   // 불꽃센서가 연결된 아날로그핀
int sound = A1;                   // 소리감지센서가 연결된 아날로그핀
int flame2 = A2;                  // 불꽃센서가 연결된 아날로그 핀 2
int sound2 = A3;                  // 소리감지센서가 연결된 아날로그 핀 2
int beep = 12;                    // 부저가 연결된 디지털핀
int flameval = 0;                 // 불꽃센서 출력값 저장 변수
int soundvol = 0;                 // 소리감지센서 출력값 저장 변수
int flameval2 = 0;                 // 불꽃센서 출력값 저장 변수
int soundvol2 = 0;                 // 소리감지센서 출력값 저장 변수
int i = 0;                        // LOOP가 몇 번 돌았는지를 체크하는 변수

void setup()
{
  pinMode(beep, OUTPUT);          // 부저 출력설정
  pinMode(flame, INPUT);          // 불꽃센서 입력설정
  pinMode(sound, INPUT);          // 소리감지센서 입력설정
  pinMode(flame2, INPUT);         // 불꽃센서2 입력설정
  pinMode(sound2, INPUT);         // 소리감지센서2 입력설정
  lcd.init();                     // LCD 초기화
  Serial.begin(9600);            // 시리얼모니터 설정

}

void loop() {

  flameval = analogRead(flame);   // 불꽃센서에서 값을 읽어옴
  soundvol = analogRead(sound);   // 소리감지센서에서 값을 읽어옴
  flameval2 = analogRead(flame2);  // 불꽃센서2에서 값을 읽어옴
  soundvol2 = analogRead(sound2);  // 소리감지센서2에서 값을 읽어옴
  
  Serial.println(flameval);       // 시리얼 모니터로 값 확인
  Serial.println(flameval2);      // 시리얼 모니터로 값 확인
  Serial.println(soundvol);       // 시리얼 모니터로 값 확인
  Serial.println(soundvol2);      // 시리얼 모니터로 값 확인

if(soundvol > 200 || flameval > 2 ){
    digitalWrite(beep, HIGH);     // 부저 BEEP
    
    lcd.setCursor(0, 0);          //  LCD커서 위치
    lcd.print("FIRE IN ROOM 1");  // 1번 방 화재문구
    i = 0;                        // 작동이 잘 됬으니 LOOP횟수 0으로 초기화
    lcd.backlight();              // 백라이트 ON
    delay(3000);                  // 3초간
    lcd.noBacklight();            // 백라이트 OFF
}

else if(soundvol2 > 200 || flameval2 > 2){
    digitalWrite(beep, HIGH);     // 부저 BEEP
    
    lcd.setCursor(0, 1);          // LCD 커서 위치
    lcd.print("FIRE IN ROOM 2");  // 2번방 화재문구
    i = 0;                        // 작동이 잘 됬으니 LOOP횟수 0으로 초기화
    lcd.backlight();              // LCD백라이트 ON
    delay(3000);                  // 3 초간 유지
    lcd.noBacklight();            // LCD백라이트 OFF
}

else{
      digitalWrite(beep, LOW);      // 부저 OFF
    lcd.noBacklight();            // LCD 백라이트 끄기
    lcd.clear();                  // LCD 문자열 삭제
}



  while (i > 3000)                // ROOP가 50번 이상 돌았을때(점검 기간)
  {
    lcd.backlight();            // LCD 백라이트 ON
    lcd.setCursor(0, 0);        // LCD 커서위치 이동
    lcd.print("Need to Check"); // 소화 센서 점검문구
    lcd.setCursor(0, 1);        // LCD 커서 위치 이동
    lcd.print("ROOM 1 and 2");  // 소화 센서 점검문구
  }
  i++;
  delay(500);                   // 딜레이 0.5초
}
