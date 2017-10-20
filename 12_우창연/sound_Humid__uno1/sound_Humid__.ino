#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int redled = 10;
int yellowled = 9 ;
int blueled = 8;
int greenled = 7; 


void setup() {

  Serial.begin(9600);                                       // 시리얼 통신을 시작, 통신 속도는 9600



    // initialize the LCD

 

  // Turn on the blacklight and print a message.
 
}






void loop() {

  int a = analogRead(A0);                               // 정수형 변수 a를 선언하고 A0핀에 입력되는 신호를 대입




  if (a > 20) {                                                // 만약 a가 150보다 크다면

    Serial.print(a);                                           // 시리얼 모니터에 a의 값을 출력

    delay(100);                                               // 0.1초 대기

  }

//이 밑 부분은 ++를 표시해주는 코드입니다.

  if (20 < a && a < 400 ) {

    Serial.println("  ++");

  }



  if (400 < a && a < 650 ) {

    Serial.println("  +++++");

  }

  if (650 < a && a < 900 ) {

    Serial.println("  ++++++++++");

  }

  if (900 < a && a < 1023 ) {

    Serial.println("  ++++++++++++++");

  }

 
  
delay(2000);

  int h = dht.readHumidity();

  int t = dht.readTemperature();

  Serial.print("습도: ");

  Serial.print(h);

  Serial.print(" %\t");

  Serial.print("온도: ");

  Serial.print(t);

  Serial.println(" C");

  if ( t > 10 )
  {
    digitalWrite( redled, HIGH);
  }
  else { digitalWrite( redled, LOW);
  }

if ( h > 50 )
  {
    digitalWrite( yellowled, HIGH);
  }
  else { digitalWrite( yellowled, LOW);
  }

  if ( a > 80 )
  {
    digitalWrite( blueled, HIGH);
  }
  else { digitalWrite(  blueled, LOW);
  }
  



}


