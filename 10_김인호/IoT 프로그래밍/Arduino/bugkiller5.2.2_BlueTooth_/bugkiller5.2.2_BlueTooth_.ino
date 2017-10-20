#include <Servo.h>
#include <SoftwareSerial.h>

#define RxD 12
#define TxD 11

Servo m1;
SoftwareSerial bt(RxD,TxD); // 블루투스 연결

int pos = 90; // 서보모터 이동 각

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(3,INPUT); // 3번 포트에 푸시버튼 연결
pinMode(RxD,INPUT);
pinMode(TxD,OUTPUT);
m1.attach(9); // 9번 포트에 서보모터 연결
m1.write(pos); // pos(90)도로 이동(한후에 핀으로 각도를 맞춤)
}

void loop() {
  // put your main code here, to run repeatedly:
  int d = digitalRead(3); // 3번 포트에 푸시버튼 연결
  char input; // 블루투스로 입력받음
  
  Serial.print("about_switch : ");
  Serial.print(d ); // 스위치 상태
  Serial.print("pos : ");
  Serial.println(pos); // 서보모터 각도

  if(bt.available() > 0)
  {
    if(bt.read() != '#')
    { return; }
    if(input == 1)
    {
      while(pos < 180)
      {
        Serial.print("about_switch : ");
        Serial.print(d ); // 스위치 상태
        Serial.print("pos : ");
        Serial.println(pos); // 서보모터 각도

        m1.write(pos);
        delay(10);
        pos += 5;
        if(pos == 180)
        {
          break;
        }
      }
    }

    else if(input == 2)
    {
      while(pos > 0)
      {
        Serial.print("about_switch : ");
        Serial.print(d ); // 스위치 상태
        Serial.print("pos : ");
        Serial.println(pos); // 서보모터 각도

        m1.write(pos);
        delay(10);
        pos -= 5;
        if(pos == 0)
        {
          break;
        }
      }
    }

    else
    {
      while(1)
      {
        Serial.print("about_switch : ");
        Serial.print(d ); // 스위치 상태
        Serial.print("pos : ");
        Serial.println(pos); // 서보모터 각도

        if(pos < 90)
        {
          m1.write(pos);
          delay(10);
          pos += 5;
        }
        else if(pos > 90)
        {
          m1.write(pos);
          delay(10);
          pos -= 5;
        }
        else
        {
          m1.write(pos);
          break;      
        }
      }
    }
    Serial.write(input);
  }
}
