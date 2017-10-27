#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // LCD
#include "LedControl.h" 
#include <SPI.h>              //RFID  
#include <MFRC522.h>         //RFID
#include "LedControl.h"                
#include <MsTimer2.h>

  
// RFID
MFRC522 rfid(10, 9);
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];
int checkcode[4] = {184,83,21,39};
boolean ConnectMode = 0;

//LCD
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address

// LED Control * Joystick
LedControl lc=LedControl(7,6,4,1); // set the matrix led pin
int select_position = 2;
boolean jscount =0;
int push_cnt=0;
int ONOFF[7] = {1,1,1,1,1,1,1};
int selectONOFF_cnt = 0;

//블루투스 핀 설정
SoftwareSerial mySerial(2, 3);
int led_state[6] = {0,0,0,0,0,0};

//시간측정
int time_flag = 0;
int time_cnt = 0;


void setup()
{
  Bluetooth_init();
  lcd.init();
  lcd.backlight(); // LCD initialize
  LED_init();
  lc_init(); // LEDControl initialize
  RFID_init(); //RFID initalize

  MsTimer2::set(200, Timer);
  MsTimer2::start();   
  
}

void loop()
{
  RFID();
 if(ConnectMode == 1)
 {
   time_flag = 1;
   joystick_move();
   if(time_cnt>=100)
   {
    time_flag=0;
    ConnectMode=0;
    time_cnt=0;
    mySerial.write('X');
   }
 }
  else
  {
  lcd.clear();
  lc.clearDisplay(0);
 }
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void Timer()
{
     jscount=0;
     push_cnt=0;
     if(time_flag==1)
     {
      time_cnt++;
     }
}

void lc_init()
{
  lc.shutdown(0,false); 
  lc.setIntensity(0,5);    
  lc.clearDisplay(0);   
}

void RFID_init()
{
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

    for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}

void LED_init()
{
  lc.shutdown(0,false); 
  lc.setIntensity(0,5);    
  pinMode(5, INPUT);    
 
  pinMode(5, INPUT);
  digitalWrite(5, HIGH);
}


void Bluetooth_init()
{  
  Serial.begin(9600); // 시리얼 통신의 속도를 9600으로 설정
  while (!Serial) {
    ;
  }
  mySerial.begin(9600);  //블루투스와 아두이노의 통신속도를 9600으로 설정
    
}

////////////////////////////////////////////////////////////////////////// RFID

void RFID()
{
    // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    lcd.clear();
    lcd.print("Wrong input");
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    
    ConnectMode = 0;
    lcd.clear();
    lcd.print("A new card has");
    lcd.setCursor(0, 1);
    lcd.print(" been detected.");
    delay(300);
    
    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    
    lcd.clear();
    lcd.print("ID Check...");
    delay(300);
    
  // 입력된 RFID카드가 기존에 입력된 RFID인지 확인하는 함수

   if (nuidPICC[0] == checkcode[0] || 
    nuidPICC[1] == checkcode[1]|| 
    nuidPICC[2] == checkcode[2]|| 
    nuidPICC[3] == checkcode[3]) 
    {
    ConnectMode = 1;
    lcd.clear();
    lcd.print("Wait...");
    LED_init_state();
    lcd.clear();
    lcd.print("allowed!");
    lcd.setCursor(0, 1);
    lcd.print("     Connected");
    }

    else
    {
    lcd.clear();
    lcd.print("not allowed!");
    delay(300);
    ConnectMode = 0;
    }
    

  }
  else
  {
      lcd.clear();
      lcd.print("Card read ");
      lcd.setCursor(0, 1);
      lcd.print("     previously.");
      delay(300);
    if (nuidPICC[0] == checkcode[0] || 
    nuidPICC[1] == checkcode[1]|| 
    nuidPICC[2] == checkcode[2]|| 
    nuidPICC[3] == checkcode[3]) 
    {
    ConnectMode = 1;
    lcd.clear();
    lcd.print("Wait...");
    LED_init_state();
    lcd.clear();
    lcd.print("allowed!");
    lcd.setCursor(0, 1);
    lcd.print("     Connected");
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

////////////////////////////////////////////////////////////////////////// LED & Joystick


void LED_init_state()
{
  
mySerial.write('O');
 
for(int i=0;i<6;i++)
{
  while(!mySerial.available()){
    ;
  }
  led_state[i]=mySerial.read();
}

  if(led_state[0] ==65)
  {
  lc.setLed(0,0,0,true);
  lc.setLed(0,0,1,true);
  lc.setLed(0,1,0,true);
  lc.setLed(0,1,1,true); // 1생활관
  ONOFF[1] = 1;
  }
  else  ONOFF[1] = 0;

  if(led_state[1] ==66)
  {  
  lc.setLed(0,3,0,true);
  lc.setLed(0,4,0,true);
  lc.setLed(0,3,1,true);
  lc.setLed(0,4,1,true); // 2생활관
  ONOFF[2] = 1;
  }
  else   ONOFF[2] = 0;

  if(led_state[2] ==67)
  {    
  lc.setLed(0,6,0,true);
  lc.setLed(0,7,0,true);
  lc.setLed(0,6,1,true);
  lc.setLed(0,7,1,true); // 3생활관
  ONOFF[3] = 1;
  }
  else   ONOFF[3] = 0;

  if(led_state[3] ==68)
  {     
  lc.setLed(0,0,7,true);
  lc.setLed(0,0,6,true);
  lc.setLed(0,1,7,true);
  lc.setLed(0,1,6,true); // 4생활관
  ONOFF[4] = 1;
  }
  else  ONOFF[4] = 0;

  if(led_state[4] ==69)
  {    
  lc.setLed(0,3,7,true);
  lc.setLed(0,3,6,true);
  lc.setLed(0,4,7,true);
  lc.setLed(0,4,6,true); // 5생활관
  ONOFF[5] = 1;
  }
  else   ONOFF[5] = 0;

  if(led_state[5] ==70)
  {    
  lc.setLed(0,6,7,true);
  lc.setLed(0,6,6,true);
  lc.setLed(0,7,7,true);
  lc.setLed(0,7,6,true); // 6생활관
  ONOFF[6] = 1;
  }
  else ONOFF[6] = 0;
  
}

void LED_ON(int number)
{
  int value = number;
  switch(value){
    case 1:
        lc.setLed(0,0,0,true);
        lc.setLed(0,0,1,true);
        lc.setLed(0,1,0,true);
        lc.setLed(0,1,1,true); // 1생활관
        ONOFF[1] = 1;
        mySerial.write('A');
        lcd.clear();
        lcd.print("Room 1 ON");
        delay(300);
        lcd.clear();
    break;
    case 2:
        lc.setLed(0,3,0,true);
        lc.setLed(0,4,0,true);
        lc.setLed(0,3,1,true);
        lc.setLed(0,4,1,true); // 2생활관
        ONOFF[2] = 1;
        mySerial.write('B');
        lcd.clear();
        lcd.print("Room 2 ON");
        delay(300);
        lcd.clear();
    break;
    case 3:
       lc.setLed(0,6,0,true);
       lc.setLed(0,7,0,true);
       lc.setLed(0,6,1,true);
       lc.setLed(0,7,1,true); // 3생활관
        ONOFF[3] = 1;
        mySerial.write('C');
        lcd.clear();
        lcd.print("Room 3 ON");
        delay(300);
        lcd.clear();
    break;
    case 4:
       lc.setLed(0,0,7,true);
       lc.setLed(0,0,6,true);
       lc.setLed(0,1,7,true);
       lc.setLed(0,1,6,true); // 4생활관
        ONOFF[4] = 1;
        mySerial.write('D');
        lcd.clear();
        lcd.print("Room 4 ON");
        delay(300);
        lcd.clear();     
    break;
    case 5:
       lc.setLed(0,3,7,true);
       lc.setLed(0,3,6,true);
       lc.setLed(0,4,7,true);
       lc.setLed(0,4,6,true); // 5생활관
       ONOFF[5] = 1;
        mySerial.write('E');
        lcd.clear();
        lcd.print("Room 5 ON");
        delay(300);
        lcd.clear();
    break;
    case 6:
       lc.setLed(0,6,7,true);
       lc.setLed(0,6,6,true);
       lc.setLed(0,7,7,true);
       lc.setLed(0,7,6,true); // 6생활관
       ONOFF[6] = 1;
        mySerial.write('F');
        lcd.clear();
        lcd.print("Room 6 ON");
        delay(300);
        lcd.clear();
    break;    
    
    default:
    break;
  }
}

void LED_OFF(int number)
{
  int value = number;
  switch(value){
    case 1:
        lc.setLed(0,0,0,false);
        lc.setLed(0,0,1,false);
        lc.setLed(0,1,0,false);
        lc.setLed(0,1,1,false); // 1생활관
        ONOFF[1] = 0;
        mySerial.write('a');
        lcd.clear();
        lcd.print("Room 1 OFF");
        delay(300);
        lcd.clear();
    break;
    case 2:
        lc.setLed(0,3,0,false);
        lc.setLed(0,4,0,false);
        lc.setLed(0,3,1,false);
        lc.setLed(0,4,1,false); // 2생활관
        ONOFF[2] = 0;
        mySerial.write('b');
        lcd.clear();
        lcd.print("Room 2 OFF");
        delay(300);
        lcd.clear();
    break;
    case 3:
       lc.setLed(0,6,0,false);
       lc.setLed(0,7,0,false);
       lc.setLed(0,6,1,false);
       lc.setLed(0,7,1,false); // 3생활관
       ONOFF[3] = 0;
        mySerial.write('c');
        lcd.clear();
        lcd.print("Room 3 OFF");
        delay(300);
        lcd.clear();
    break;
    case 4:
       lc.setLed(0,0,7,false);
       lc.setLed(0,0,6,false);
       lc.setLed(0,1,7,false);
       lc.setLed(0,1,6,false); // 4생활관
       ONOFF[4] = 0;
        mySerial.write('d');
        lcd.clear();
        lcd.print("Room 4 OFF");
        delay(300);
        lcd.clear();
    break;
    case 5:
       lc.setLed(0,3,7,false);
       lc.setLed(0,3,6,false);
       lc.setLed(0,4,7,false);
       lc.setLed(0,4,6,false); // 5생활관
       ONOFF[5] = 0;
        mySerial.write('e');
        lcd.clear();
        lcd.print("Room 5 OFF");
        delay(300);
        lcd.clear();
    break;
    case 6:
       lc.setLed(0,6,7,false);
       lc.setLed(0,6,6,false);
       lc.setLed(0,7,7,false);
       lc.setLed(0,7,6,false); // 6생활관
       ONOFF[6] = 0;
        mySerial.write('f');
        lcd.clear();
        lcd.print("Room 6 OFF");
        delay(300);
        lcd.clear();
    break;    
    
    default:
    break;
  }
}

void LED_center_off()
{
    lc.setColumn(0, 3, 0);
    lc.setColumn(0, 4, 0);
}

void LED_select()
{
  switch(select_position){
  case 1:
  lc.setLed(0,0,3,true);
  lc.setLed(0,1,3,true); // 1생활관 앞 표시
  break;

  case 2:
  lc.setLed(0,3,3,true);
  lc.setLed(0,4,3,true); // 2생활관 앞 표시
  break;

  case 3:
  lc.setLed(0,6,3,true);
  lc.setLed(0,7,3,true); // 3생활관 앞 표시
  break;

  case 4:
  lc.setLed(0,0,4,true);
  lc.setLed(0,1,4,true); // 4생활관 앞 표시
  break;

  case 5:
  lc.setLed(0,3,4,true);
  lc.setLed(0,4,4,true); // 5생활관 앞 표시
  break;

  case 6:
  lc.setLed(0,6,4,true);
  lc.setLed(0,7,4,true); // 6생활관 앞 표시
  break;

  default:
  break;
}
}

void joystick_move()
{
  int push = digitalRead(5);                      
  int X = analogRead(1);                           
  int Y = analogRead(0);                          
  int temp = 0;
  if(jscount==0)
  {
    if(push==0&&push_cnt==0)
    {
       selectONOFF(select_position);
       push_cnt  = 1;
       Serial.println(push);
    }
    
    else if(Y>800||Y<300)
      {
        if(Y>800){
          temp = select_position +1;
          if(temp>=7)
          {
            select_position=1;
          }
          else
          {
            select_position = select_position +1;
          }
        }
        else{
          temp = select_position -1;
          if(temp<=0)
          {
           select_position=6;
          }
          else
          {
           select_position = select_position -1;
          }
        }
      }
      else if(X>800||X<300)
      {
              if(select_position==4)
              {
                select_position=1; 
              }
              else if(select_position==5)
              {
                select_position=2;
              }
              else if(select_position==6)
              {
                select_position=3;
              }
              else if(select_position==1)
              {
                select_position=4;
              }
              else if(select_position==2)
              {
                select_position=5;
              }
              else if(select_position==3)
              {
                select_position=6;
              }
          
        }
      

     LED_center_off();
     LED_select();
      }

   jscount =1;   
}


void selectONOFF(int number){

  if(ONOFF[number]==1)
  {
    LED_OFF(number);
  }
  else if(ONOFF[number]==0)
  {
    LED_ON(number);
  }
}


