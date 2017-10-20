#include <DS1302RTC.h>
#include <Time.h>
#include <TimeLib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h> 
#include <LCD.h>
#include <LiquidCrystal_I2C.h>



//Define Buzzer
#define BUZZ_PIN 12

//Define DHT
#define DHTTYPE DHT11
#define DHTPIN 2

//Define RTC
#define RTC_CLK 6
#define RTC_DAT 7
#define RTC_RST 8
#define DS1302_GND_PIN 5
#define DS1302_VCC_PIN 4

//Define LCD
//ALWAYS USE THIS WITH LCD I2C and Addres 0x3F
#define I2C_ADDR 0x3F
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

//Define RBG
#define RGB_R_PIN 9
#define RGB_G_PIN 10
#define RGB_B_PIN 11



//create DHT
DHT dht(DHTPIN,DHTTYPE);

//create RTC
DS1302RTC RTC(RTC_RST,RTC_DAT,RTC_CLK);

//create CLD
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);




//RGB based on Eff
void rgb_on(int eff){
  float valr;
  float valg;
   
  valr = map(100-eff,0,100,0,255);
  valg = map(255-valr,0,255,0,20); //green too strong

  /*Serial.print("valr : ");
  Serial.println(valr);
  Serial.print("valrg : ");
  Serial.println(valg);
  Serial.println("--------");*/
  
  analogWrite(RGB_R_PIN, valr);
  analogWrite(RGB_G_PIN, valg);
  return;
}



//calcuate summer waste, return eff
int calcSE(float temp_user, float temp_recommended){
  float E_waste = (temp_recommended - temp_user)/temp_recommended*100;

  E_waste = constrain(E_waste,0,100); //E_waste value less than 0 is not wasting energy; E_waste above 100 is impossible
  E_waste = map(E_waste,0,43,0,100); //a/c lat most 16 degrees; highest recom temp 28; 28-16/28 = 43% -> worst case 100% waste

  /*Serial.print("Waste : ");
  Serial.println(E_waste);
  Serial.print("UTemp : ");
  Serial.println(temp_user);
  Serial.println("-------");*/

  return 100-E_waste;
}

//calcuate winter waste, return eff
int calcWE(float temp_user, float temp_recommended){
  float E_waste = (temp_user - temp_recommended)/temp_recommended*100;

  E_waste = constrain(E_waste,0,100); //E_waste value less than 0 is not wasting energy; E_waste above 100 is impossible
  E_waste = map(E_waste,0,78,0,100); //heater at most 32 degrees; lowest recom temp 18; 32-18/18 = 78% -> worst case 100% waste

  /*Serial.print("Waste : ");
  Serial.println(E_waste);
  Serial.print("UTemp : ");
  Serial.println(temp_user);
  Serial.println("-------");*/

  return 100-E_waste;
}



//change status
void changestat(String &stat, int eff){
  if(0 <= eff && eff < 36) { stat = "BAD"; }
  else if(36 <= eff && eff < 68) { stat = "AVERAGE"; }
  else if(68 <= eff && eff <= 100) {stat = "GOOD"; } 
  return;
}



// setup time
void setupTime(tmElements_t &tm, time_t &t){
  int datetime[5];
  
  Serial.println("Please set the current date and time according to the following format\n");
  Serial.print("YY : ");
  while(Serial.available() == 0){}
  datetime[0] = Serial.parseInt();
  Serial.print(datetime[0]); Serial.println(" ");
  Serial.print("MM : ");
  while(Serial.available() == 0){}
  datetime[1] = Serial.parseInt();
  Serial.print(datetime[1]); Serial.println(" ");
  Serial.print("DD : ");
  while(Serial.available() == 0){}
  datetime[2] = Serial.parseInt();
  Serial.print(datetime[2]); Serial.println(" ");
  Serial.print("HH : ");
  while(Serial.available() == 0){}
  datetime[3] = Serial.parseInt();
  Serial.print(datetime[3]); Serial.println(" ");
  Serial.print("mm : ");
  while(Serial.available() == 0){}
  datetime[4] = Serial.parseInt();
  Serial.print(datetime[4]); Serial.println("\n");

  tm.Year = y2kYearToTm(datetime[0]);
  tm.Month = datetime[1];
  tm.Day = datetime[2];
  tm.Hour = datetime[3];
  tm.Minute = datetime[4];

  t = makeTime(tm);
  
  //use the time_t value to ensure correct weekday is set
  if(RTC.set(t) == 0) { // Success  
    setTime(t);
    Serial.println("RTC set to: ");
    printDateTime(t);
    Serial.println("\n\n");
  }
  else{
    Serial.println();
    Serial.println("RTC set failed!");
    //dump any extraneous input
    while (Serial.available() > 0) Serial.read();
  }
    
  t = now();
  return;

}


// setup intro
int setup(float &rtemp){
  int input = 0;
  int choice;

  while(input != 1 || input != 2){
    Serial.println("Please select the season for temperature control :");
    Serial.println("1) Summer (Hot Weather)     2) Winter (Cold Weather)");
    Serial.print("Please select 1 or 2 : ");
    while(Serial.available() == 0){}
    input = Serial.parseInt();
    Serial.println(input);
    if(input == 1 || input == 2){
      choice = input;
    }

    switch (input){
      case 1:
           Serial.println("What would you like the ideal temperature to be?");
           Serial.println("Recommended temperature for hot weather is 26 ~ 28 degrees Celsious");
           Serial.print("Please choose a number between 26 and 28 : ");
           while(Serial.available() == 0){}
           input = Serial.parseInt();
           Serial.println(input);
           if(input >= 26 && input <= 28) { Serial.println("\n"); }
           else { Serial.println("Invalid input!\n"); input = 0; }
           break;

      case 2:
           Serial.println("What would you like the ideal temperature to be?");
           Serial.println("Recommended temperature for cold weather is 18 ~ 20 degrees Celsious");
           Serial.print("Please choose a number between 18 and 20 : ");
           while(Serial.available() == 0){}
           input = Serial.parseInt();
           Serial.println(input);
           if(input >= 18 && input <= 20) { Serial.println("\n"); }
           else { Serial.println("Invalid input!\n"); input = 0; }
           break;
           
      default:
           Serial.println("Invalid input!\n");
           input = 0;
           break;
      }
    if(input != 0) {Serial.println("Done!\n\n\n"); rtemp = input; break;}
  }
  return choice;
}



//print date and time to Serial
void printDateTime(time_t t)
{
   printI00(day(t), 0);
   Serial.print(monthShortStr(month(t)));
   Serial.println(year(t), DEC);
   printI00(hour(t), ':');
   printI00(minute(t), ' ');
}

//Print an integer in "00" format (with leading zero),
//followed by a delimiter character to Serial.
//Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10) Serial.print("0");
    Serial.print(val, DEC);
    
    if (delim > 0) Serial.print(delim);
    
    return;
}

void activateRTC(){
  digitalWrite(DS1302_GND_PIN, LOW);
  pinMode(DS1302_GND_PIN, OUTPUT);
  digitalWrite(DS1302_VCC_PIN, HIGH);
  pinMode(DS1302_VCC_PIN, OUTPUT);

  /*Serial.println("RTC module activated");
  delay(500);
  if (RTC.haltRTC()) {
    Serial.println("The DS1302 is stopped.  Please set time");
    Serial.println("to initialize the time and begin running.");
    Serial.println();
  }
  if (!RTC.writeEN()) {
    Serial.println("The DS1302 is write protected. This normal.");
    Serial.println();
  }*/
  
  delay(5000);
    
  //setSyncProvider() causes the Time library to synchronize with the
  //external RTC by calling RTC.get() every five minutes by default.
  setSyncProvider(RTC.get);

  Serial.println("RTC Sync");
  if (timeStatus() == timeSet)
    Serial.println(" Ok!");
  else
    Serial.println(" FAIL!");

  Serial.println();
}



//notify user function
void notify(String stat, time_t t){
  if(stat == "BAD"){
    tone(BUZZ_PIN,500,1000);
    Serial.println("\nEnergy waste!! Please adjust temperature!");
    printDateTime(t);
    Serial.println();
  }
}



//Variables
int choice;

float rtemp; //recommended temperature
float temp; //Stores temperature value
int eff;
String status;

time_t t;
tmElements_t tm;



void setup()
{

  //begin serial
  Serial.begin(9600);
  dht.begin();


  //activate RTC
  activateRTC();


  //setup LCD
  lcd.begin(16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  lcd.print("Must setup on PC");

  Serial.println("Welcome to Arduino Temperature Alarm!\n");

  //set time
  setupTime(tm, t);

  //setup for winter vs summer
  choice = setup(rtemp);

  //reset RBG
  pinMode(RGB_R_PIN, OUTPUT);
  pinMode(RGB_G_PIN, OUTPUT);
  pinMode(RGB_B_PIN, OUTPUT);
  analogWrite(RGB_R_PIN, 0);
  analogWrite(RGB_G_PIN, 0);
  analogWrite(RGB_B_PIN, 0);

  
}

void loop()
{
  
  temp = dht.readTemperature();
  
  if(choice == 1){
    eff = calcSE(temp,rtemp);
  }
  else if(choice == 2){
    eff = calcWE(temp,rtemp);
  }
  changestat(status, eff);


  //Print temp and eff values to serial monitor
  /*Serial.print("Eff: ");
  Serial.print(eff);
  Serial.print(" %, Temp: ");
  Serial.print(temp);
  Serial.println(" Celsius");*/


  //LCD display
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Eff: ");
  lcd.print(eff);
  lcd.print("%");
  lcd.print(" ");
  lcd.print(status);


  //RBG
  rgb_on(eff);  

  //notify user
  notify(status, t);


  delay(5*60*1000); //Delay 5 min

}

