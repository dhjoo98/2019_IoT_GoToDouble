#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define TRIG_PIN 2
#define ECHO_PIN 3
#define BUZZER 6

int lcount = 0;

//RFID
#include <SPI.h>
#include <Timer.h>
#include <AddicoreRFID.h>

#define MAX_LEN 16
#define uchar unsigned char
#define uint unsigned int

#define RFID_SDA 10
#define RFID_SCK 13
#define RFID_MOSI 11
#define RFID_MISO 12
#define RFID_RST 9

uchar serNumA[5];
uchar fifobytes;
uchar fifoValue;

AddicoreRFID cardRead;
Timer ts;
unsigned int smaplingT = 10;
boolean catrfid = false;





LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int count = 0;
bool check = false;
float max_distance = 0;
float distance;
float duration;
bool start = true;
int cycle = 0;
unsigned long time_loss = 0;



void setup() { 
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();

   Serial.begin(9600);
  SPI.begin();

  pinMode(RFID_SDA,OUTPUT);
  pinMode(RFID_RST,OUTPUT);
   digitalWrite(RFID_SDA,LOW);
  digitalWrite(RFID_RST,HIGH);

  cardRead.AddicoreRFID_Init();
  
}
 
void loop() 
{
  if (lcount > 9) {
    lcount = 0;
    }
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Connect Card");
  delay(5000); 

  
 while(true){
  uchar status;
  uchar str[MAX_LEN];

  str[1] = 0x4400;
  
    status = cardRead.AddicoreRFID_Request(PICC_REQIDL, str);
    status = cardRead.AddicoreRFID_Anticoll(str);
    if ( status == MI_OK ){
      lcd.clear();
      lcd.setCursor(2,0);
      lcd.print("17-76002226");
      lcd.setCursor(1,1);
      lcd.print("Han Jin Tak");
      delay(5000);
      int distance = 0;  
      count = 0;
        do{
          cycle++;
        for (int i = 3; i >= 1; i--){
          lcd.clear();
          lcd.setCursor(8, 0);
          lcd.print(i);
          delay(1000);
          }
        digitalWrite(TRIG_PIN, HIGH);
        delay(10);
        digitalWrite(TRIG_PIN, LOW);
        duration = pulseIn(ECHO_PIN, HIGH);
        max_distance = ((34000*duration)/1000000)/2;
        lcd.setCursor(0,0);
        lcd.print("max distance is");
        lcd.setCursor(0,1);
        lcd.print(max_distance);
        lcd.setCursor(6,1);
        lcd.print("cm");
        delay(3000);
        
    
        }while(30 > max_distance || max_distance > 50);
       /* 10cm 이내로 접근 시 LED를 켠다 */  
      for (int i = 3; i >= 1; i--){
          lcd.clear();
          lcd.setCursor(8, 0);
          lcd.print(i);
          delay(1000);
          }
          lcd.setCursor(6,0);
          lcd.print("START");
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("COUNT");
          lcd.setCursor(2,1);
          lcd.print(count);
          lcd.setCursor(11,0);
          lcd.print("TIME");
          time_loss = millis();
     do{
      digitalWrite(TRIG_PIN, HIGH);
      delay(10);
      digitalWrite(TRIG_PIN, LOW);
      duration = pulseIn(ECHO_PIN, HIGH);
      distance = ((34000*duration)/1000000)/2;
    
     lcd.setCursor(12,1);
     lcd.print((millis() - time_loss)/1000);
      
      if (distance >= max_distance - 0.8) { 
        
        if (check == true){
        count++;
        lcd.setCursor(6,1);
          lcd.print("DOWN");
        
        lcd.setCursor(7,0);
          lcd.print("  ");
        
        lcd.setCursor(2,1);
          lcd.print(count);
         check = false;
        tone(BUZZER, 500, 250);                   
        delay(500);
        
        }
      }
      else if (distance <= 10){
        check = true;
        
        lcd.setCursor(6,1);
          lcd.print("    ");
        
        lcd.setCursor(7,0);
          lcd.print("UP");
        
        delay(300);
        
        } 
      }
      while(millis() - time_loss < 120000 / 4 );
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" Your Record is");
      lcd.setCursor(7,1);
      lcd.print(count);
      Serial.print(lcount);
      Serial.print(",");
      Serial.print(str[0]);
      Serial.print(str[1]);
      Serial.print(str[2]);
      Serial.print(",");
      Serial.print(0);
      Serial.print(",");
      Serial.println(count);
    }
  
  
  delay(500);
  }
  delay(5000);
  lcount++;
}



