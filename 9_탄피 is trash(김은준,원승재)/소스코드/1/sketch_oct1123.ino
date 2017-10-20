#include <SoftwareSerial.h>
#include <Keypad.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);
SoftwareSerial mySerial(2, 3); //블루투스의 tx,rx 설정

const byte ROWS = 4; // 4 Rows
const byte COLS = 4; // 4 Columns 
char hexaKeys[ROWS][COLS] = {  
{ '1', '5', '9', 'C' }, 
{ '2', '6', '0', 'D' }, 
{ '3', '7', 'A', 'E' }, 
{ '4', '8', 'B', 'F' }
};
byte rowPins[ROWS] = { 6, 7, 8, 9 }; // ROW 열에 연결할 핀
byte colPins[COLS] = { 10, 11, 12, 13 }; // COLUMN 열에 연결할 핀
 //Keypad 인스턴스 생성
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

char buf[2];
int z=0;
int x=0;
long done;
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;  }
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    lcd.init();
    lcd.backlight();
  
    lcd.print("Shooting control");

    Serial.println("사격통제");

  //블루투스와 아두이노의 통신속도를 9600으로 설정
    mySerial.begin(9600);
}

void loop() { //코드를 무한반복합니다.
  char customKey='x';
  customKey=customKeypad.getKey();
  if(mySerial.available())
  {
    lcd.clear();
    done=mySerial.read();
    Serial.println(done);
    lcd.print("Rest Bullet :");
    lcd.print(done);
    
    if(done==99)
    {
       lcd.clear();
       lcd.print("  - E n d - ");
       
    }
  }
  switch(customKey)
  {
    case 'A':
        lcd.clear();
        Serial.println("전송");
        Serial.println(z);
        mySerial.write(z);
         
        lcd.print(" S t a r t !");
        delay(1000);
        lcd.clear();
        lcd.print("Rest Bullet :");
        lcd.print(z);
        
        break;
    case 'B':
        int cnt=0;
        
        Serial.println("탄은 두자리 숫자로 입력");
        Serial.println("탄수 결정 :");
        lcd.clear();
        for(;;)
        { 
          
          customKey=customKeypad.getKey();
          if(customKey)
          {
            Serial.print(customKey);
            buf[cnt]=customKey;
            cnt++;
            Serial.println();
 
          }
          if(cnt==2)
          {
            z=atoi(buf);
            lcd.clear();
            Serial.print("입력된 탄수 :");
            Serial.println(z);
            Serial.println("입력완료");
            lcd.print("Bullet:");
            lcd.print(z);
            break;
          }
    
       }
 }
  
}


