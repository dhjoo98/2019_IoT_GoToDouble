#include "TimerOne.h"
#include <Wire.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#define Blue_A 8
#define Red_A 7
//#define UserC3 63
#define UD_Level 70
#define PIN_LED 7

int Timer = 1 ;    // 세탁기가 켜져서 돌고있는지를 저장하는 변수
int count = 3;      //예약자수 저장 변수
int SEC = 0 ;
int MIN =0 ;
int HOU =0 ;
int FINISH = 75;    //세탁기안의 세탁물을 가져갔는지 를 저장하는 변수

//int receiveData =1;

SoftwareSerial mySerial(2, 3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
const int rs = 12, en = 11, d4 = 2, d5 = 5, d6 = 4, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void ReceiveHandler(int data)                           //I2C통신으로 값을 수신했을 때 실행되는 함수
{
  while(Wire.available()>0){
  I2C_Read();
  Serial.println("What the fuckin integer");
  }
  delay(3000);
}
void Handler ()
{
  Serial.print("What the fucking is it");
  delay (3000);
}
//--------------------------------------I2C 통신으로 값을 읽어서 해당되는 변수에 데이터 저장
void I2C_Read()
{
    char Buffer =0;
    Buffer = Wire.read();
    Serial.print("리더 버퍼");
    Serial.print(Buffer);
    Buffer -=160;
     
      Serial.println(Buffer,DEC);
       if(Buffer>74)                       //데이터가 75이상이면 피니시 데이터로 인식하여 저장
       {
        FINISH = Buffer;
        Serial.println("in The FINISH line");
       }
       else if(Buffer>60){                  // 데이터가 60~74 사이이면 카운트 데이터로 인식하여 저장
            count = Buffer-UD_Level;
            Serial.println("In The user");
          }
  
      else if(Buffer>0)                         //데이터가 0~60이면 시간데이터로 인식하여 저장
      {                                          //마스터에서 넘어오는 시간데이터는 초, 분, 시 순으로 수신됨
        Serial.println("In The Time");
        if((HOU==0)&&(MIN==0)&&(SEC==0))
          SEC = Buffer;
        else if((HOU==0)&&(MIN==0))
          MIN = Buffer;
        else if((HOU==0))
          HOU = Buffer;
        else
        Serial.println("모두 차 있음");
      }
      else
      {
        Serial.println("blueTooth ReadERR");
      }
}
/*
void BT_Readaing_LED()
{
       lcd.begin(16,2);                           // 블루투스 리딩화면 표시
       //lcd.backlight();                      // 블루투스 리딩화면 표시
       lcd.setCursor(2,0);                   // 블루투스 리딩화면 표시
       lcd.print("Receving BT...");           // 블루투스 리딩화면 표시
}*/
/*
void B_T_Read()
{
  Serial.println("In The BT_Read");
  char Buffer = 0;
  Buffer =mySerial.read();
  Serial.println(Buffer,DEC);
       if(Buffer>74)
       {
        FINISH = Buffer;
        Serial.println("in The FINISH line");
       }
       else if(Buffer>60){            // 데이터가 유저카운트 레벨일 경우
            count = Buffer-UD_Level;
            Serial.println("In The user");
          }
  
      else if(Buffer>0)                        //데이터가 타이머 레벨일 경우
      { 
        Serial.println("In The Time");
        if((HOU==0)&&(MIN==0)&&(SEC==0))
          SEC = Buffer;
        else if((HOU==0)&&(MIN==0))
          MIN = Buffer;
        else if((HOU==0))
          HOU = Buffer;
        else
        Serial.println("모두 차 있음");
      }
      else
      {
        Serial.println("blueTooth ReadERR");
      }
   

}*/
/*
void LCD_refresh(){
  for(;;){
    lcd.begin(16,2);
    lcd.setCursor(3,0);
    lcd.print("Running");
    lcd.setCursor(3,1);
    lcd.print("Time : ");
    lcd.print(SEC);
    
  }
}*/
//------------------------------LCD에 시간 및 예약번호 프린트
void LCD_Print()
{
    lcd.setCursor(0,0);
    lcd.print("T");lcd.print("i");lcd.print("m");lcd.print("e");lcd.print(" ");
    lcd.setCursor(5,0);
    lcd.print(HOU/10);
    lcd.print(HOU%10);
    lcd.print(":");
    lcd.print(MIN/10);
    lcd.print(count);
    lcd.begin(16,2); 
}
//------------------------Serprint()
void SerPrint()
{

                Serial.print(HOU);
                Serial.print("  ");
                Serial.print(MIN);
                Serial.print("  ");
                Serial.print(SEC);
                Serial.println("  ");
}
//------------------------시간을 카운트 다운해주는 인터럽트
void callback()
{
         if((SEC ==0)&&(MIN==0)&&(HOU==0)){
            if(Timer==true){
             count--;
            }Timer=0; // 타이머0값 전달
           
   //        Serial.println("타이머 0");
         }
           else if ((MIN==0)&&(SEC==0))
           {   
               SerPrint();

            HOU--;
            MIN=59;
            SEC=59;
 //           Serial.println("IN 시");

           }
           else if (SEC==0)
           {
                SerPrint();

           MIN--;
           SEC=59;
//           Serial.println("IN 분");

           }
           else// if(SEC>0)
           {
 //           Serial.println("IN 초");
                SerPrint();
  
           SEC--;
           Timer=true;
           FINISH=false;
            }
            
}

void setup() {
  Wire.begin(Red_A);                       //자신의 I2C 통신 주소 설정
  Wire.onReceive(ReceiveHandler);           //I2C로 데이터 수신시 이벤트 함수설정
  Wire.onRequest(Handler);
  lcd.begin(16,2); //추가
  //lcd.backlight(); 
  Timer1.initialize(1000000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt
  pinMode(PIN_LED , OUTPUT);
  pinMode(13,OUTPUT);                   //부저 핀모드 설정
    // 시리얼 통신의 속도를 9600으로 설정
  Serial.begin(9600);
  while (!Serial) {
    ; //시리얼통신이 연결되지 않았다면 코드 실행을 멈추고 무한 반복
  }
    mySerial.begin(9600);

}


void loop(){

  if((Timer==0)&&(FINISH==75)) //초기상태 ready 출력  //세탁기 끝났고, 안에 세탁물도 없는상태 -> 초기상태
  {
    lcd.begin(16,2);
    lcd.print("r");
    lcd.print("e");
    lcd.print("a");
    lcd.print("d");
    lcd.print("y");
  }
  if(Timer>0){                              // 세탁기 가동상태
     LCD_Print();
     digitalWrite(PIN_LED, HIGH);
     //Serial.println("카운트 on");
  }
  if((Timer==0)&&(FINISH==0))               // 세탁기 꺼짐 BUT 안에 가져가지 않은 세탁물 존재
  {                                         // 세탁물을 가져가면 블루투스로 FINISH 데이터를 75로 받아 해당 if절 우회됨 
    lcd.begin(16,2);
    lcd.print("w");
    lcd.print("a");
    lcd.print("i");
    lcd.print("t");

    digitalWrite(PIN_LED, LOW);             //led 설정
    delay(1000);
    digitalWrite(13,HIGH);                  //부저
    delay(1000);
    digitalWrite(13,LOW);
//    Serial.println("카운트 0");
  
  }/*
  Serial.print("타이머 ");
 Serial.println(Timer);
 Serial.print("피니시  ");
 Serial.println(FINISH);
 
*/
  if(Wire.available())
  {
    Serial.println("와이어 어베일러블!");
    
  }
  
  
}
