#include "SoftwareSerial.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <DHT11.h>

Servo locker; 
LiquidCrystal_I2C lcd(0x3F, 16, 2); 
DHT11 dht11(11);
char secretCode[4] = {'1', '2', '3', '4'};
char input[4];
char key;
int position = 0;
int wrong = 0;
int i = 0;
int j = 0;
int pos = 0;
int a = 0;
int b = 0;
int count = 0;
const byte rows = 4;
const byte cols = 4;
boolean c = false;

//키패드
char keys[rows][cols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[rows] = {9, 8, 7, 6};
byte colPins[cols] = {5, 4, 3, 1};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

void input_key();
void password();
void password_check();
void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  locker.attach(10);
  locker.write(90);
  locker.attach(10);
}
void loop()
{
  //초기화면
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Do's");
  lcd.setCursor(0, 1);
  lcd.print("16-71005962");

  key = keypad.waitForKey();
  //온습도
  if (key == '*')
  {

    int err;
    float humi;
    float tem;


    if ((err = dht11.read(humi, tem)) == 0) {


      char humiCharValue[10];
      dtostrf(humi, 4, 1, humiCharValue);


      String humiDisplayStr = "Humidity: ";
      humiDisplayStr += (String)humiCharValue;
      humiDisplayStr += " %";

      Serial.println(humiDisplayStr);
      lcd.setCursor(0, 0);

      lcd.print(humiDisplayStr);


      char temCharValue[10];
      dtostrf(tem, 4, 1, temCharValue);


      String temDisplayStr = "Temper  : ";
      temDisplayStr += (String)temCharValue;
      temDisplayStr += "'C";
      Serial.println(temDisplayStr);
      lcd.setCursor(0, 1);
      lcd.print(temDisplayStr);
      delay(2000);
    }

    else {
      Serial.println();
      Serial.print("Error No :");
      Serial.print(err);
      Serial.println();
    }

  }
 //비밀번호 입력창
  if (key == '#')
  {
    input_key();
    password();
    password_check();
  }
  int sum = a;
  if (sum == 1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);

    locker.write(0);
    lcd.print("perfect complete");
    lcd.setCursor(0, 1);
    lcd.print("Lock, press 0");
    key = keypad.waitForKey();
    if (key == '0')
    {
      a = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      locker.write(90);
      lcd.print("Lock ON");
      Serial.println("Do`s Lock ON");
      delay(2000);
      count = 0;
    }

  }

}
//비밀번호 입력
void input_key()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Password");
  for (i = 0; i < 4; i++)
  {
    key = keypad.waitForKey();
    input[i] = key;
    lcd.setCursor(i + 6, 1);
    lcd.print("*");
    if (i == 3)
    {
      Serial.println("");
    }
  }
}
//비밀번호 생성
void password()
{
  wrong = 0;
  for (j = 0; j < 4; j++)
  {
    if (secretCode[j] == input[j])
    {
    }
    else if (secretCode[j] != input[j])
    {
      wrong += 1;
    }
  }
}

void password_check()
{
  //비밀번호가 맞으면 나오는 화면
  if (wrong == 0) 
  {
    Serial.println("Do`s correct");
    lcd.clear();
    lcd.print("      PASS");
    delay(1000);
    a = 1;
    lcd.clear();
  }
  //비밀번호가 틀리면 나오는 화면
  else  if (wrong != 0) 
  {
    Serial.println("Do`s wrong");
    lcd.clear();
    lcd.print("     wrong");
    delay(1000);
    a = 0;
    lcd.clear();
    wrong = 0;
    count += 1;
  }
  // 3번 틀리면 나오는 화면
  if (count == 3) { 
    Serial.println("Do`s Waring");
    lcd.clear();
    lcd.print("Waring");
    delay(5000);
    count = 0;
  }
}




