#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>
#include <MFRC522.h>
#include "TimerOne.h"

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define Y_LED           7
#define B_LED           6
#define R_LED           5
#define PUSH            4
#define FINISH          75
#define ArduinoY        0
#define ArduinoB        80
#define ArduinoR        160


SoftwareSerial mySerial(2, 3); //블루투스의 Tx, Rx핀을 2번 3번핀으로 설정
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
LiquidCrystal_I2C lcd(0x3F,16,2); // LCD Setting

//시간 변수 선언
int SEC;
int MIN;
int HOU;
int timer = HOU+MIN+SEC;

//예약 관련 변수 선언
int user_count = 0;      // 세탁기 사용자 수 count
int iden_user=-1;        // 사용자의 RFID 번호
int iden_reserved1 = -1; //첫째 예약 카드 RFID 번호
int iden_reserved2 = -1; //둘째 예약 카드 RFID 번호

//-----------------------------------------------------------------------------------setup함수----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void setup() {
    lcd.init();
    lcd.backlight(); //lcd backlight on
    mySerial.begin(9600);
  	Serial.begin(9600);		// Initialize serial communications with the PC
  	while (!Serial);		    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  	SPI.begin();	     		  // Init SPI bus
  	mfrc522.PCD_Init();		  // Init MFRC522
  
    pinMode(Y_LED, OUTPUT);      //LED 핀번호 설정
    pinMode(B_LED, OUTPUT);
    pinMode(R_LED, OUTPUT);
    pinMode(PUSH, INPUT_PULLUP); //스위치 핀번호 설정
    digitalWrite(Y_LED, LOW);    //LED 등 초기화
    digitalWrite(B_LED, LOW);
    digitalWrite(R_LED, LOW);
    Timer1.initialize(1000000);  //1000000=1초를 1초씩 count
 } 

//---------------------BT 송신 함수: user_count 및 time---------------------
void Trans_State_Time_data_to_Y(){
    //user_count += 70;
    mySerial.write((user_count + 70 + ArduinoY));
    mySerial.write(SEC + ArduinoY);
    mySerial.write(MIN + ArduinoY);
    mySerial.write(HOU + ArduinoY);
}

void Trans_State_Time_data_to_B(){
    //user_count += 70;
    mySerial.write((user_count + 70 + ArduinoB));
    mySerial.write(SEC + ArduinoB);
    mySerial.write(MIN + ArduinoB);
    mySerial.write(HOU + ArduinoB);
}

void Trans_State_Time_data_to_R(){
  //user_count += 70;
    mySerial.write((user_count + 70 + ArduinoR));
    mySerial.write(SEC + ArduinoR);
    mySerial.write(MIN + ArduinoR);
    mySerial.write(HOU + ArduinoR);
}

//---------------------BT 송신 함수: time---------------------
void Trans_Time_data_to_Y(){
    mySerial.write(SEC + ArduinoY);
    mySerial.write(MIN + ArduinoY);
    mySerial.write(HOU + ArduinoY);
}

void Trans_Time_data_to_B(){
    mySerial.write(SEC + ArduinoB);
    mySerial.write(MIN + ArduinoB);
    mySerial.write(HOU + ArduinoB);
}

void Trans_Time_data_to_R(){
    mySerial.write(SEC + ArduinoR);
    mySerial.write(MIN + ArduinoR);
    mySerial.write(HOU + ArduinoR);
}

//---------------------스위치 입력 대기 함수---------------------
void switch_input_standby(){
    lcd.init();
    lcd.print("Press SW");
  
    Serial.println("전원 버튼을 눌러주세요.");  
    for(;;){
        if (!(digitalRead(PUSH))){                    //왜 인지 모르겠지만 안누른 상태가 HIGH이기 때문에 누르면 LOW 신호가 돼서 not연산으로 통해 break 
            break;
        }
        delay(100);
    }
    delay(50);
}

//---------------------RFID 입력 대기 함수---------------------
void rfid_input_standby(){
    for(;;){
        if (mfrc522.PICC_IsNewCardPresent()){
            delay(1);
            break;
        }
    }
    if (!mfrc522.PICC_ReadCardSerial()){ return; }// Select one of the cards
    delay(500);
}

//---------------------RFID 카드 구분 함수---------------------
int rfid_iden(){
    int status;

    boolean y_status = (mfrc522.uid.uidByte[0] == 0x24 && mfrc522.uid.uidByte[1] == 0x3D && mfrc522.uid.uidByte[2] == 0xE4 && mfrc522.uid.uidByte[3] == 0x18);
    boolean b_status = (mfrc522.uid.uidByte[0] == 0xA8 && mfrc522.uid.uidByte[1] == 0x06 && mfrc522.uid.uidByte[2] == 0xE1 && mfrc522.uid.uidByte[3] == 0x27);
    boolean r_status = (mfrc522.uid.uidByte[0] == 0x98 && mfrc522.uid.uidByte[1] == 0x8A && mfrc522.uid.uidByte[2] == 0x26 && mfrc522.uid.uidByte[3] == 0x27);

    
    if(y_status) {
        status = 1;
    }
    else if(b_status){
        status = 2;
    }
    else if(r_status){
        status = 3;
    }
    else {
        Serial.println("등록된 카드가 아닙니다.");
        status = 0;
    }
    return status;
}

//---------------------LCD에 시간 출력 하는 함수---------------------
void LCD_Print(){
    lcd.setCursor(0,0);
    lcd.print("T");lcd.print("i");lcd.print("m");lcd.print("e");lcd.print(" ");
    lcd.setCursor(5,0);
    lcd.print(HOU/10);
    lcd.print(HOU%10);
    lcd.print(":");
    lcd.print(MIN/10);
    lcd.print(MIN%10);
    lcd.print(":");
    lcd.print(SEC/10);
    lcd.print(SEC%10);
    lcd.setCursor(2,1);
}

//---------------------시간을 시리얼 통신으로 찍는 함수---------------------
void SerPrint(){
    Serial.print(HOU);
    Serial.print("  ");
    Serial.print(MIN);
    Serial.print("  ");
    Serial.print(SEC);
    Serial.println("  ");
    timer = HOU+MIN+SEC;
}

//---------------------타이머 함수---------------------
void callback(){
    if((SEC ==0)&&(MIN==0)&&(HOU==0)){
        SerPrint();
        delay(1000);
    }
    else if((MIN==0)&&(SEC==0)){
        SerPrint();
        HOU--;
        MIN=59;
        SEC=59;
    }
    else if (SEC==0){
        SerPrint();
        MIN--;
        SEC=59;
    }
    else{
        SerPrint();
        SEC--;
    }
}

//---------------------세탁기 on 함수---------------------
void washer_on(int rfid_uid){
    switch_input_standby();
    lcd.init();
    lcd.print("Start Washing");
    switch(rfid_uid){
        case 1: digitalWrite(Y_LED, HIGH);
                Timer1.attachInterrupt(callback);
                break;
        case 2: digitalWrite(B_LED, HIGH);
                Timer1.attachInterrupt(callback);
                break;
        case 3: digitalWrite(R_LED, HIGH);
                Timer1.attachInterrupt(callback);
                break;
        case 0: return;
    }
}

//---------------------세탁기 off 함수---------------------
void washer_off(int rfid_uid){
    
    //BT 전송
    Serial.println("세탁이 끝났습니다."); //LCD 출력
}

//---------------------RFID에 맞는 LED 선택하는 함수---------------------
int whoseLED(int rfid_uid){
    int LED;
    switch(rfid_uid){
        case 1: LED = Y_LED;
                break;
        case 2: LED = B_LED;
                break;
        case 3: LED = R_LED;
                break;
    }
    return LED;
}

//-----------------------------------------------------------------------------------loop함수----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void loop() {
 while(true){ // 마지막에 continue; 쓰기 위해서 loop()내 전체 함수를 while(true)로 묶음

     // 타이머 설정 및 초기화
     SEC = 20;
     MIN = 0;
     HOU = 0;
     
     
// 제 1장 대기자가 없으면 그냥 실행하고 대기자가 있으면 대기자 카드만 세탁기 실행 가능
     if(iden_user == -1){                                        // 대기자가 없는 최초 세탁기 실행
         lcd.init();
         lcd.print("Tag your card");
         
         Serial.println("리모컨의 카드를 인식기에 대주세요");
         rfid_input_standby();                                   // RFID 인식 대기
         iden_user = rfid_iden();                                // 카드 고유 번호 저장
         if(!(iden_user)){
          lcd.init();
          lcd.print("Undefined card");
          iden_user = -1;
          return;                                                // 등록된 카드가 아니면 처음으로
         }
         washer_on(iden_user);                                   // 세탁기 켜는 함수(LED on, 타이머 작동, LCD 출력)
         user_count = user_count +1;                             // 사용 인원 현황 최신화
         switch(iden_user){                                      // 카드에 따라 블루투스에 따로 보냄.
             case 1: Trans_State_Time_data_to_Y();               // 해당 리모컨으로 BT 연결해서 on 된 것만 전송(slave는 블루투스 연결되면 LED on, 타이머 작동)
                     break;
             case 2: Trans_State_Time_data_to_B();
                     break;
             case 3: Trans_State_Time_data_to_R();
                     break;
         }
     }
     else if(iden_user != -1){                                   // 대기자가 있을시 첫 대기자 카드만 세탁기 실행 가능
         Serial.println("1번째 대기자의 카드를 인식기에 대주세요");

         lcd.init();
         lcd.print("Tag 1st reserved");
         lcd.setCursor(0,1);
         lcd.print("CARD.");
         
         rfid_input_standby();                                   // RFID 인식 대기
         int id_find = rfid_iden();                              // 카드 고유 번호 저장
        
         if(id_find == iden_user) {                              // 첫 대기자 카드면 세탁기 open
             Serial.println("세탁기가 열렸습니다.");
             washer_on(iden_user);
             switch(iden_user){                                  // 카드에 따라 블루투스에 따로 보냄.
                 case 1: Trans_State_Time_data_to_Y();           // 해당 리모컨으로 BT 연결해서 on 된 것만 전송(slave는 블루투스 연결되면 LED on, 타이머 작동)
                         break;
                 case 2: Trans_State_Time_data_to_B();
                         break;
                 case 3: Trans_State_Time_data_to_R();
                         break;
             }
             if(iden_reserved1 != -1){                           //두번째 대기자한테도 보냄
                  switch(iden_reserved1){                        // 카드에 따라 블루투스에 따로 보냄.
                      case 1: Trans_Time_data_to_Y();            // 해당 리모컨으로 BT 연결해서 on 된 것만 전송(slave는 블루투스 연결되면 LED on, 타이머 작동)
                              break;
                      case 2: Trans_Time_data_to_B();
                              break;
                      case 3: Trans_Time_data_to_R();
                              break;
                  }
             }
         }
         else{                                                   //다른 카드는 인식 안됨
             LCD_Print();
             lcd.print("Not 1st reserver");
             Serial.println("대기자만 세탁 시작이 가능합니다.");
             continue;
         }
     }

   
// 제 2장 세탁 중일 때 다른 RFID 오면 예약, 사용자 RFID오면 인식 무시, 미등록 카드 무시)
     while(timer){                                                                                             // timer가 아직 0이 아닐때(즉, 세탁 중 일때)
         delay(1);
         LCD_Print();
         if(mfrc522.PICC_IsNewCardPresent()){                                                                  // RFID 카드 인식 대기
             rfid_input_standby();
             int iden_new = rfid_iden();
       
             if(!(iden_new)) {                                                                                 // 미등록 카드 인식 안함
                 Serial.println("등록 되지 않은 카드입니다.");
                 LCD_Print();
                 lcd.print("Unknown card");
                 continue;                                                                                     // while(timer) 처음으로
             }
             else if(iden_new  == iden_user){                                                                  //사용자 카드면 인식 안함
                 LCD_Print();
                 lcd.print("Please wait");
                 Serial.println("아직 세탁 중입니다. 기다려 주십시요.");
                 continue;
             }
             else if(iden_new == iden_reserved1 || iden_new == iden_reserved2){                                 // 예약자 카드면 인식 안함.
                 LCD_Print();
                 lcd.print("Already reserved");
                 Serial.println("이미 예약 되었습니다.");
                 continue;
             }
             else if(iden_new != iden_user || iden_new != iden_reserved1 || iden_new != iden_reserved2){        // 사용자, 예약자 카드가 아니면 예약 실행

                 user_count=user_count+1;

                 LCD_Print();
                 lcd.setCursor(0,1);
                 lcd.print("Reserved");
                 lcd.print(user_count-1);
                 lcd.setCursor(0,0);
                 
                 Serial.println("예약되었습니다");
                 Serial.print("당신은 ");
                 Serial.print(user_count-1);
                 Serial.print("번째 대기자입니다.");
                 delay(1000);
                 
                 switch(iden_new){                                        // 카드에 맞는 블루투스에 타이머 및 대기 번호 보냄.
                     case 1: Trans_State_Time_data_to_Y();                // 해당 리모컨으로 BT 연결해서 on 된 것만 전송(slave는 블루투스 연결되면 LED on, 타이머 작동)
                             break;
                     case 2: Trans_State_Time_data_to_B();
                             break;
                     case 3: Trans_State_Time_data_to_R();
                             break;
                 }
                 
                 if(iden_reserved1 < 0){                                  // 예약자 정보 관리(예약되면 새카드 정보를 예약 변수에 넣음.)
                     iden_reserved1 = iden_new;
                     continue; //while(timer)로
                 }
                 else if(iden_reserved2 < 0){                             // 예약자 정보 관리(예약 되면 새카드 정보를 예약 변수에 넣음.)
                     iden_reserved2 = iden_new;
                     continue; //while(timer)로
                 }
             }
         }
     delay(1);
     }
 
  
// 제 3장 timer가 0이 되면(세탁이 다 되면) 세탁기 끄기
     while(!(timer)){ // timer가 0이 된 상황
         Timer1.detachInterrupt(); //callback 함수 호출 해제
         user_count=user_count-1; //user_count down
         washer_off(iden_user); //세탁기 off(LCD 출력, BT 전송)
       
         lcd.init();
         lcd.print("FINISHED");
         lcd.setCursor(0,1);
         lcd.print("Tag your card");
         lcd.setCursor(0,0);
         
         Serial.println("세탁물을 꺼내려면 카드를 대주세요"); // RFID 카드 대면 세탁기 열리고 LED off
         while(1){ // RFID 대기 전까지 LED Blink
             int LED = whoseLED(iden_user);
             if(mfrc522.PICC_IsNewCardPresent()){ // 카드 인식됨.
                 rfid_input_standby(); // RFID 인식 대기
                 int id_find = rfid_iden(); // 카드 고유 번호 저장
        
                 if(id_find == iden_user) { //카드 대면 LED 꺼지기
                     lcd.init();
                     lcd.print("Door Opend");
                     lcd.setCursor(0,1);
                     lcd.print("Get your clothes");
                     delay(1500);
                     Serial.println("세탁기가 열렸습니다. 세탁물을 가져가세요.");
                     digitalWrite(LED, LOW);
                     break;
                 }
                 else{ //다른 카드는 인식 안됨
                     lcd.init();
                     lcd.print("Cant open Door");
                     lcd.setCursor(0,1);
                     lcd.print("Tag user card");
                     lcd.setCursor(0,0);
                     
                     Serial.println("세탁기를 열 수 없습니다.");
                     continue;
                 }
             }
             delay(1);
     
             digitalWrite(LED, HIGH);  //Blink 구현
             delay(1000);
             digitalWrite(LED, LOW);
             delay(1000);
         
         }//blink 끝

         switch(iden_user){                                              // 사용자 리모컨에 FINISH 값 보냄
                 case 1: mySerial.write(FINISH + ArduinoY);                   
                         break;
                 case 2: mySerial.write(FINISH + ArduinoB);
                         break;
                 case 3: mySerial.write(FINISH + ArduinoR);
                         break;
         }
         if(iden_reserved1 != -1){                                        // 첫 대기자가 있으면 FINISH 값 보냄
              switch(iden_reserved1){                
                 case 1: mySerial.write(FINISH + ArduinoY);
                         break;
                 case 2: mySerial.write(FINISH + ArduinoB);
                         break;
                 case 3: mySerial.write(FINISH + ArduinoR);
                         break;
              }
         }
         if(iden_reserved2 != -1){
              switch(iden_reserved2){                                     // 둘쨰 대기자가 있으면 FINISH 값 보냄
                 case 1: mySerial.write(FINISH + ArduinoY);
                         break;
                 case 2: mySerial.write(FINISH + ArduinoB);
                         break;
                 case 3: mySerial.write(FINISH + ArduinoR);
                         break;
              }
         }
         
         iden_user = iden_reserved1; // 순서 변경
         iden_reserved1 = iden_reserved2;
         iden_reserved2 = -1;   
         break;
     }//while(!(timer))끝
     continue;
 }//while(true) 끝
}//loop() 끝
