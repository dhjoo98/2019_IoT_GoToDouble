#include <Servo.h>

Servo m1;

int pos = 90; // 서보모터 이동 각
int count = 0; // 중복 방지

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(3,INPUT); // 3번 포트에 푸시버튼 연결
m1.attach(9); // 9번 포트에 서보모터 연결
m1.write(pos); // pos(90)도로 이동(한후에 핀으로 각도를 맞춤)
}

void loop() {
  // put your main code here, to run repeatedly:
  int b = analogRead(A1); // A1 포트에 조도센서 연결
  int d = digitalRead(3); // 3번 포트에 푸시버튼 연결
  
  //Serial.print("about_switch : ");
  //Serial.print(d ); // 스위치 상태
  Serial.print("pos : ");
  Serial.print(pos); // 서보모터 각도
  Serial.print("bright : ");
  Serial.println(b); // 조도
  
  if(b < 500) // 어두울 때만 작동함
  {
    if((d == HIGH) && (count == 0))
    {
      while(pos < 180)
      {
        //Serial.print("about_switch : ");
        //Serial.print(d ); // 스위치 상태
        Serial.print("pos : ");
        Serial.print(pos); // 서보모터 각도
        Serial.print("bright : ");
        Serial.println(b); // 조도

        m1.write(pos);
        delay(10);
        pos += 5;
        if(pos == 180)
        {
          count = 1;
          break;
        }
      }
    }
    
    else if((d == HIGH) && (count == 1))
    {
      while(pos > 0)
      {
        //Serial.print("about_switch : ");
        //Serial.print(d ); // 스위치 상태
        Serial.print("pos : ");
        Serial.print(pos); // 서보모터 각도
        Serial.print("bright : ");
        Serial.println(b); // 조도

        m1.write(pos);
        delay(10);
        pos -= 5;
        if(pos == 0)
        {
          count = 0;
          break;
        }
      }
    }

    else
    {
      while(1)
      {
        //Serial.print("about_switch : ");
        //Serial.print(d ); // 스위치 상태
        Serial.print("pos : ");
        Serial.print(pos); // 서보모터 각도
        Serial.print("bright : ");
        Serial.println(b); // 조도

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
  }
  
  else
  {
    while(1)
    {
      //Serial.print("about_switch : ");
      //Serial.print(d ); // 스위치 상태
      Serial.print("pos : ");
      Serial.print(pos); // 서보모터 각도
      Serial.print("bright : ");
      Serial.println(b); // 조도

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
}
