#include <Servo.h>

Servo m1;

int pos = 90; // 서보모터 이동 각
int count = 0; // 중복 방지

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(3,INPUT); // 3번 포트에 택트 스위치 연결
m1.attach(9); // 9번 포트에 서보모터 연결
m1.write(pos); // pos(90)도로 이동(한후에 핀으로 각도를 맞춤)
}

void loop() {
  // put your main code here, to run repeatedly:
  int d = digitalRead(3); // 3번 포트에 택트 스위치 연결
  
  Serial.print("switch_state : ");
  Serial.print(d ); // 스위치 상태
  Serial.print("pos : ");
  Serial.println(pos); // 서보모터 각도
  
  if((d == HIGH) && (count == 0)) // 택트 스위치가 눌렸는데 Off 상태일 때
  {
    while(pos < 180) // 180도가 될때 까지 회전
    {
      Serial.print("switch_state : ");
      Serial.print(d ); // 스위치 상태
      Serial.print("pos : ");
      Serial.println(pos); // 서보모터 각도

      m1.write(pos);
      delay(10);
      pos += 9;
      if(pos == 180) // 180도가 되면
      {
        count = 1; // 스위치 On 상태
        break;
      }
    }
  }

  else if((d == HIGH) && (count == 1)) // 택트 스위치가 눌렸는데 On 상태일 때
  {
    while(pos > 0) // 0도가 될때 까지 회전
    {
      Serial.print("switch_state : ");
      Serial.print(d ); // 스위치 상태
      Serial.print("pos : ");
      Serial.println(pos); // 서보모터 각도

      m1.write(pos);
      delay(10);
      pos -= 9;
      if(pos == 0) // 0도가 되면
      {
        count = 0; // 스위치 Off 상태
        break;
      }
    }
  }

  else
  {
    while(1)
    {
      Serial.print("switch_state : ");
      Serial.print(d ); // 스위치 상태
      Serial.print("pos : ");
      Serial.println(pos); // 서보모터 각도

      if(pos < 90) // 90도 보다 작을 때 90도로 돌아오도록 이동
      {
        m1.write(pos);
        delay(10);
        pos += 9;
      }
      else if(pos > 90) // 90도 보다 클 때 90도로 돌아오도록 이동
      {
        m1.write(pos);
        delay(10);
        pos -= 9;
      }
      else
      {
        m1.write(pos);
        break;      
      }
    }
  }
}
