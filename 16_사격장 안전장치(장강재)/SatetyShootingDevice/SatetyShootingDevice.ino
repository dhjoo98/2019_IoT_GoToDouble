#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH
#include <SoftwareSerial.h>   
SoftwareSerial BTSerial(12, 13);   
int segA = 2; //Display pin 
int segB = 3; //Display pin 
int segC = 4; //Display pin 
int segD = 5; //Display pin 
int segE = 6; //Display pin 
int segF = 7; //Display pin 
int segG = 8; //Display pin 
int sw1 = A2;       // 스위치(startSW) 핀 설정
int sw2 = A3;       // 스위치(errorSW) 핀 설정
int reading1;          // startSW 상태
int previous1 = LOW;   // atartSW 이전 상태
long time = 0;        // LED가 ON/OFF 토글된 마지막 시간
long debounce = 30;  // Debounce 타임 설정
int bullet =A0;
int setbullet = A1;
int shoot=0;
int red=10;
int green=11;
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(bullet, OUTPUT);
  pinMode(setbullet, INPUT);
  pinMode(sw1, INPUT_PULLUP); // SW 를 설정, 아두이노 풀업저항 사용
  pinMode(sw2, INPUT_PULLUP); // SW 를 설정, 아두이노 풀업저항 사용
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
reading1 = digitalRead(sw1);  // SW 상태 읽음
bullet = map(analogRead(setbullet),0,767,0,9);
/*Serial.println(analogRead(setbullet));*/
  switch (bullet-shoot){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
   if (reading1 == HIGH && previous1 == LOW && millis() - time > debounce) //버튼누르면 숫자감소
   {
    Serial.println("SHOOT");
    shoot=shoot+1;
    Serial.println(shoot);
    time = millis();
   }
 /*  Serial.println(bullet-shoot);
   Serial.println("NOWBULLET");*/
   previous1 = reading1;
   if(bullet==shoot && digitalRead(sw2) == HIGH) //사격완료 초록불
   {
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
    Serial.println("SHOOT COMPLETE");
    delay(2000);
   } else if(digitalRead(sw2) == LOW) //기능고장일 경우
   {
       analogWrite(green,50);
       analogWrite(red,255);
   }
  else //사격 중일 경우
   {
     digitalWrite(red,HIGH);
     digitalWrite(green,LOW);
   }
   
            if (Serial.available()) //사격완료 블루투스 전송
         {
             BTSerial.write(Serial.read());
         }
         if (BTSerial.available())
         {
             char val = (char)BTSerial.read();
             Serial.print(val);
             if((bullet-shoot)==0)
             {Serial.println(" LANE SHOOT COMPLETE(bluetooth send)");
             }
         }
    delay(600);
}


