#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define TRIG_PIN 2
#define ECHO_PIN 3
#define BUZZER 6

int lcount = 0;  // this value exist for server


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
//unsigned int smaplingT = 10;

String tmp;



LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int count = 0;
bool check = false;
float max_distance = 0;
float distance;
float duration;
bool start = true;
unsigned long time_loss = 0;

boolean outputCardInfo(){
    while(true){
    uchar status;
    uchar str[MAX_LEN];

    str[1] = 0x4400;
  
    status = cardRead.AddicoreRFID_Request(PICC_REQIDL, str);
    status = cardRead.AddicoreRFID_Anticoll(str);
    if ( status == MI_OK ){
      //Serial.print(str[0]);
      //Serial.print(str[1]);
      //Serial.print(str[2]);
      //Serial.print(",");          //  RFID 의 UID 를 넘겨주어 누구인지 파악
    tmp = str[0] + str[1] + str[2];
    break;
    }
  
  delay(500);
  
  }
  return true;
}

// RFID 카드입력


void setup() { 
  pinMode(TRIG_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);
  lcd.init();                     
  lcd.init();
  lcd.backlight();

   Serial.begin(9600);
   SPI.begin();
  
  pinMode(RFID_SDA,OUTPUT);
  pinMode(RFID_RST,OUTPUT);
   digitalWrite(RFID_SDA,LOW);
  digitalWrite(RFID_RST,HIGH); // -> LCD

  cardRead.AddicoreRFID_Init();
  
}
 
void loop() 
{
  if (lcount > 9) {           //lcount 는 서버가 앞 사람과 뒷 사람을 구별하는 변수
    lcount = 0;
    }
  Serial.print(lcount);
  Serial.print(",");

  
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Connect Card");
  delay(5000); 

  
 if(outputCardInfo()){ 
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("17-76002226");
  lcd.setCursor(1,1);
  lcd.print("Han Jin Tak");  // 가능하면 RFID 카드 접촉 시 서버에서 군번과 이름을 받아와 
  delay(5000);               // LCD 에 보여주고 싶었으나 시간부족으로 구현 못함
  int distance = 0;  
  count = 0;
    do{
    for (int i = 3; i >= 1; i--){
      lcd.clear();
      lcd.setCursor(8, 0);
      lcd.print(i);
      delay(1000);
      }
    digitalWrite(TRIG_PIN, HIGH);
    delay(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH);            //초음파센서를 이용한 거리측정
    max_distance = ((34000*duration)/1000000)/2;   //max_distance 는 팔 쭉 핀 상태에서의 거리
    lcd.setCursor(0,0);
    lcd.print("max distance is");
    lcd.setCursor(0,1);
    lcd.print(max_distance);
    lcd.setCursor(6,1);
    lcd.print("cm");                                //max_distance 값, 즉 팔 쭉 핀 상태의 거리를 보여줌
    delay(3000);
    

    }while(30 > max_distance || max_distance > 50); // 팔을 쭉 폈을 때의 보통 거리 내인지 검사
   /* 10cm 이내로 접근 시 LED를 켠다 */  
  for (int i = 3; i >= 1; i--){
      lcd.clear();           //여기서부터
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
      lcd.print("TIME");     // 여기까지 기록 측정시 기본 화면 구성
      time_loss = millis();  // time_loss 는 시작시간부터 측정시작 순간까지의 시간
 do{
  digitalWrite(TRIG_PIN, HIGH);
  delay(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = ((34000*duration)/1000000)/2;

 lcd.setCursor(12,1);
 lcd.print((millis() - time_loss)/1000);  //실시간으로 시간을 보여줌
  
  if (distance >= max_distance - 0.8) {   // - 0.8 은 최소한의 보정치
    
    if (check == true){              //올라옴
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
  else if (distance <= 10){           //내려감
    check = true;
    
    lcd.setCursor(6,1);
      lcd.print("    ");
    
    lcd.setCursor(7,0);
      lcd.print("UP");
    
    delay(300);
    
  } 
}while(millis() - time_loss < 120000 );  //millis() - time_loss 는 순수 측정 시간
lcd.clear();
lcd.setCursor(0,0);
 lcd.print(" Your Record is");
 lcd.setCursor(7,1);
 lcd.print(count);
  Serial.print(lcount);
  Serial.print(",");
  Serial.print(tmp);
 Serial.print(","); 
 Serial.print(0);
 Serial.print(",");
 Serial.println(count);
 delay(5000);
 lcount++;                    //lcount 는 서버가 앞 사람과 뒷 사람을 구별하는 변수
 }
  }             





