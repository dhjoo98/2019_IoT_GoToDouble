/* 최종 제출용 코드
 *  2017-10-19 운전병 지킴이
 */

#include <SimpleDHT.h>  // 온습도 라이브러리 // 

#include <LiquidCrystal_I2C.h>  // LCD라이브러리 //
#include <Wire.h>
#include <SoftwareSerial.h>   // 블루투스 라이브러리
#include <DS1302.h>  // 현재시각 라이브러리

/**
 * 전역변수 선언 
 **/

// 현재시각
#define SCK_PIN 5
#define IO_PIN  6
#define RST_PIN 13

DS1302 rtc(RST_PIN, IO_PIN, SCK_PIN);  // 시간객체
 
// 블루투스 
SoftwareSerial BTSerial(3,4);  

// 충격
int crush= 12;       // 스위치(SW) 핀 설정
int previous = LOW;   // SW 이전 상태
// int led_crush = 13;       // LED 핀 설정   LED 안 씀
//int state = LOW;      // LED 상태

long time = 0;        // LED??가 ON/OFF 토글된 마지막 시간
long debounce = 50;  // Debounce 타임 설정

int pinDHT11 = 2;
SimpleDHT11 dht11;

// 초음파센서 출력 : trig, 수신 : echo
int trig = 9;
int echo = 8;
int driver_count = 0; // 운전자 시간 카운트

// 피에조 부저 
int piezo = 7;

// 불꽃센서
int flame = A0;
int val = 0;

// 기울기 및 LED 센서
//const int LED = 6;
const int tilt = 10;
const int tilt2 = 11;


// LCD 인스턴스 생성
LiquidCrystal_I2C lcd(0x3F, 16, 2);  


//카운트들!
int count_r = 0;
int count_c = 0;
int count_a = 0;

void Car_count();
void Car_count2();

String car_time1;
String car_time2;
void car_time(String a, String b);

int count_sum = 0;
int count_pre = 0;


/**
 *  셋업 부분 (최초 1회 실행)
 **/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // 블루투스
  BTSerial.begin(9600);
  
  // 기울기 핀 : 10
     // 참고 : LED로 쓸 핀 : 6 을 뺐음.
  //pinMode(LED,OUTPUT);
  pinMode(tilt,INPUT);
  pinMode(tilt2,INPUT);
  // 초음파로 쓸 핀 : 8, 9
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  // 피에조로 쓸 핀 : 7
  pinMode(piezo, OUTPUT);
  // 불꽃센서로 쓸 핀 : A0
  pinMode(flame, INPUT);
  // 충격감지(틸트센서)로 쓸 핀 : 12  
       // 참고 : LED 로 쓸 핀 : 13 을 뺐음.
  pinMode(crush, INPUT_PULLUP);
  //pinMode(led_crush, OUTPUT);

  
  // 5. 6. 13번 핀 : 시간출력 
  // Clear the 1302's halt flag
  rtc.halt(false);
  // And disable write protection
  rtc.writeProtect(false);

//  Set the time and date  (최초 업로드 시 세팅완료)
   rtc.setDOW(WEDNESDAY);
   rtc.setTime(14, 54, 0); // HH, MM, SS
   rtc.setDate(19, 10, 2017);  // dd, mm, yyyy


  

  // LCD 초기화 및 빛 나오게 초기 설정
  lcd.begin();
  lcd.backlight();
  
}

/**
 *  루프 부분 (무한반복)
 **/
void loop() {

//시간 스트링
String time_string = rtc.getDateStr(); //+ " " + rtc.getDOWStr() + "\n" + rtc.getTimeStr();
//String time_string2 = rtc.getDOWStr();
String time_string3 = rtc.getTimeStr();  
  // put your main code here, to run repeatedly:

  /* 센서 값 읽어들임(측정) */
  // 1. 초음파 측정
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int distance = pulseIn(echo, HIGH) * 17 / 1000;  // 초음파 속도 = 파장의 왕복운동시간 * 170 (pulseIn의 단위가 마이크로세컨드이므로 /10000
 
if(distance > 100)
distance = 3; // 거리

//초음파 운전자
while (distance<4)
{
  distance = pulseIn(echo, HIGH) * 17 / 1000;
  driver_count += 1; // 거리가 3 미만이면 카운트 업

  Serial.println("값 출력 : ");
  Serial.println(driver_count); // 현재 카운트 몇인지
  
//  delay(1000);
  if(driver_count == 5)
  {
  BTSerial.println("------------------------------------------------");
  BTSerial.println("[*** Emergency ***]");
  BTSerial.println("Driver Awareness Ambiguity!\n");
  BTSerial.print("Time : ");
  BTSerial.print(time_string3);
  BTSerial.print(" ");
 // BTSerial.println(time_string2);
  BTSerial.println(time_string);
  BTSerial.println("------------------------------------------------");

  
    driver_count = 0;
    count_a ++;
    Car_count();
    break;
    car_time(time_string,time_string3);
  }
  if(distance == 0)
  break;
}


  // 2. 온습도 측정
  byte temperature = 0;   // 온도
  byte humidity = 0;      // 습도
  int err = SimpleDHTErrSuccess;      // 에러처리 및 정보 받아옴
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }

  // 3. 충격감지센서(틸트센서)
  int reading = digitalRead(crush);  // SW 상태 읽음

  // 4. 불꽃센서 인식
  val = analogRead(flame);    // 값이 가끔 0~200 전 정도는 나옴 (실제 라이터로 테스트한 값 확인 필요)
 
 /* LCD 표시부 */
  // LCD 표시 (차량 핸들부와 운전자 사이 거리 / 온도 및 습도)
  lcd.setCursor(0,0);
  lcd.print(distance);
  lcd.print("cm");
  lcd.setCursor(0, 1);
  lcd.print((int)temperature); lcd.print(" *C, ");
  lcd.print((int)humidity); lcd.print(" H  ");
  lcd.print(val);
  
  delay(200);
  
  lcd.clear();

  // 5. 기울기 센서 측정
  int tiltread = digitalRead(tilt);
  int tiltread2 = digitalRead(tilt2);



  /* 사고처리 로직 */
  // CASE 1 : 전복사고 발생( 조건 : 기울기센서 2개 값 중 하나가 1일때- 기울어질때 // 나머지 센서값 관계없이)

 if((tiltread == HIGH) || (tiltread2 == HIGH)) 
 {
  Serial.println(tiltread);
  Serial.println(tiltread2);
   // digitalWrite(LED,HIGH);//light up led 
   /* 사고남을 다른 서버(pc) 로 블루투스 전송 */
   
   Serial.println("켜짐");
  BTSerial.println("------------[Car Accident Occur]-----------");
  BTSerial.print(" 1. Accident Time : ");
  BTSerial.print(time_string3);
  BTSerial.print(" ");
 // BTSerial.println(time_string2);
  BTSerial.println(time_string);
  BTSerial.println(" 2. Car Info : Military Big Bus / Num : 1 ");
  BTSerial.print(" 3. Accident Type : ");
  BTSerial.println("Car Overturn !!! \n");
  BTSerial.println("------------------------------------------");
  
    count_r ++;
    Car_count();
     car_time(time_string,time_string3);

 }

 // CASE 2. 충돌사고 발생(조건 : 틸트 스위치(reading) 가 눌려졌을 때(==LOW)  // 나머지 센서값 관계 없이

// new code
if ( (reading == LOW) && (millis() - time > debounce) ) // reading 과 previous 가 다르면...

//if ( (reading != previous) && (millis() - time > debounce) ) // reading 과 previous 가 다르면...
{
  //state = !(state);

  BTSerial.println("------------[Car Accident Occur]-----------");
  BTSerial.print(" 1. Accident Time : ");
  BTSerial.print(time_string3);
  BTSerial.print(" ");
 // BTSerial.println(time_string2);
  BTSerial.println(time_string);
  BTSerial.println(" 2. Car Info : Military Big Bus / Num : 1 ");
  BTSerial.print(" 3. Accident Type : ");
  BTSerial.println("Car Crash  !!! \n");
  BTSerial.println("------------------------------------------");
    
//        // 부저 울리기 (5번)
//        for(int i=0; i<5; i++)
//        {
//            noTone(piezo);
//            tone(piezo, 3000, 2000);
//            delay(100);
//            tone(piezo, 3200, 2000);
//            delay(100);
//            tone(piezo, 3300, 2000);
//            delay(100);
//            tone(piezo, 3500, 2000);
//            delay(1000);
//        }
//        noTone(piezo);
        time = millis();
        count_c ++;
        Car_count();
         car_time(time_string,time_string3);
}
  //digitalWrite(led_crush, state);  
  previous = reading;

  

  
  // CASE 3. 화재 발생(조건 : 불꽃센서 값이 600 이상이면      // 테스트 통한 확인 필요함
  if(val >= 600)
  {
      Serial.print("화재발생 : ");
      Serial.println(val);

  BTSerial.println("------------[Car Accident Occur]-----------");
  BTSerial.print(" 1. Accident Time : ");
  BTSerial.print(time_string3);
  BTSerial.print(" ");
 // BTSerial.println(time_string2);
  BTSerial.println(time_string);
  BTSerial.println(" 2. Car Info : Military Big Bus / Num : 1 ");
  BTSerial.println(" 3. Accident Type : ");                                      
  BTSerial.print("Fire  !!! \n");
  BTSerial.println("------------------------------------------");
  
      for(int i=0; i<5; i++){
            tone(piezo, 3951, 2000);
            delay(500);
            tone(piezo, 2793, 2000);
            delay(500);
        }
        noTone(piezo);
  }
  
if ( BTSerial.available() > 0 )
{
  char msg = NULL;
  msg = BTSerial.read();
  Serial.println(msg);

  if ( msg == 'a')
  {

    BTSerial.println("------------[Car Accident report]-----------");
  //최초
  BTSerial.println(" 1. Accident Time : ");
  BTSerial.print(car_time2);
  BTSerial.print(" ");
  BTSerial.println(car_time1);
  //마지막
  BTSerial.println("          ->          ");
  BTSerial.print(time_string3);
  BTSerial.print(" ");
  BTSerial.println(time_string);
  
  BTSerial.println(" 2. Car Info : Military Big Bus / Num : 1 ");
  BTSerial.println(" 3. Accident Type : ");
  Car_count();
  BTSerial.println("------------------------------------------");
  
  BTSerial.println("\n\nYOUR STATE\n\n");
        if(count_r > 0)
        BTSerial.println("OVERTURN");
        if(count_c > 0)
        BTSerial.println("CRASH");
        if(count_a > 0)
        BTSerial.println("AWARENESS AMBIGUITY");      
  count_r = 0;
  count_c = 0;
  count_a = 0;
  BTSerial.println("\n\n\nRESET\n\n\n");
  
}
}

count_sum = count_r + count_c + count_a;

if (count_sum > count_pre)
{
  for(int i=0; i<5; i++){
            tone(piezo, 3951, 2000);
            delay(500);
            tone(piezo, 2793, 2000);
            delay(500);
        }
        noTone(piezo);

        BTSerial.println("\n\nYOUR STATE\n\n");
        if(count_r > 0)
        BTSerial.println("OVERTURN");
        if(count_c > 0)
        BTSerial.println("CRASH");
        if(count_a > 0)
        BTSerial.println("AWARENESS AMBIGUITY");      
}
count_pre = count_sum;

}

void Car_count()
{
 BTSerial.println("[Total Car accident]");
 BTSerial.print("   Overturn :  ");
 BTSerial.println(count_r);
 BTSerial.print("   Crash :     ");
 BTSerial.println(count_c);
 BTSerial.print("   Awareness Ambiguity: ");
 BTSerial.println(count_a);
}
void car_time(String a, String b)
{
  if ( car_time1 == NULL )
    car_time1 = a.substring(0);
  if ( car_time2 == NULL )
    car_time2 = b.substring(0);
}


