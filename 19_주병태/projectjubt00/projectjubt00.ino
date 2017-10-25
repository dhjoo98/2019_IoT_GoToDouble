#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"                           // DHT.h 라이브러리를 포함한다.
#define DHTPIN 7                          // DHTPIN을 디지털 2번핀으로 정의한다.
#define DHTTYPE DHT11              // DHTTYPE을 DHT11로 정의한다.
DHT dht(DHTPIN, DHTTYPE);       // DHT설정 - (디지털2, DHT11)
int speakerpin = 8; //스피커가 연결된 디지털핀 설정
SoftwareSerial BTSerial(2, 3); //Connect HC-06 TX,RX

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup()
{
   BTSerial.begin(9600);
   Serial.begin(9600);                  // 시리얼 통신을 시작한다. 속도는9600
	// initialize the LCD
	lcd.begin();
  pinMode(8,OUTPUT);
  pinMode(12,OUTPUT);
}

void loop()
{
  if (BTSerial.available())
  Serial.write(BTSerial.read());
  if (Serial.available())
  BTSerial.write(Serial.read());

  
	delay(3000);                                // 3초 딜레이
  int h = dht.readHumidity();         // 변수 선언 (h는 습도)
  int t = dht.readTemperature();     // 변수 선언 (t는 온도)
  String stat;

   
  
  switch(h/10)  {
    case 10:
    lcd.setCursor(8,1);
    lcd.print("too high");
    
    break;
     case 9:
    lcd.setCursor(8,1);
    lcd.print("too high");
    
    break;
    case 8:
    lcd.setCursor(8,1);
    lcd.print("too high");
    
    break;
    case 7:
    lcd.setCursor(8,1);
    lcd.print("high    ");
    
    break;
    case 6:
    lcd.setCursor(8,1);
    lcd.print("high    ");
    
    break;
    case 5:
    lcd.setCursor(8,1);
    lcd.print("high    ");
    
    break;
    default:
    lcd.setCursor(8,1);
    lcd.print("normal   ");
   

  }


if(h>=80)                            
{
  stat="too high";
  tone(speakerpin,500,1000);  //500: 음의 높낮이(주파수), 1000: 음의 지속시간(1초)
  delay(2000);
  digitalWrite(12,HIGH);

}
else if(h>=70)                       
{
 stat="high";
 digitalWrite(12,HIGH);
}



else                                       
{
  stat="normal";
  digitalWrite(12,LOW);
  }






    
 
  
  lcd.setCursor(0,0);
  BTSerial.print("습도 : ");       // Humidity를 출력
  lcd.print("Hum:");
  BTSerial.print(h);                           // h(습도 값) 출력
  lcd.print(h); 
  BTSerial.print(" %\t");                   //  %를 출력
  lcd.print("% ");
  lcd.setCursor(7,0);
  BTSerial.print("온도 : ");    // Temperature를 출력
  lcd.print(" temp:");
  BTSerial.print(t);           // t(온도 값) 출력
  lcd.print(t);
  BTSerial.print(" ℃");                    // C 출력 후 줄바꿈
  BTSerial.print("     ");
  BTSerial.print("상태 : ");
  BTSerial.println(stat);
  lcd.print("C"); 

  lcd.setCursor(0,1);
      
  lcd.print("status:");
  
	
	
}
