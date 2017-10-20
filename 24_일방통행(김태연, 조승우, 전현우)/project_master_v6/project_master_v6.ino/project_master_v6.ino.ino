#include <MsTimer2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

SoftwareSerial slaveA_port(10,11);
LiquidCrystal_I2C lcd(0x3F, 16, 2);

int button_pin = 4;
//////////////// master -> slave 전송 문자////////////// 
const char master_car_in = 'c';
const char master_traffic_Green = 'g';
const char master_traffic_Red = 'r';
const char master_traffic_Yellow = 'y';
  
  /////////////slave -> master 전송 문자///////////////////
const char slave_sign_in = 'i';
const char slave_sign_out = 'o';
const char slave_sign_wait = 'w';
const char slave_sign_passive = 'p';
const char slave_sign_auto = 'a'; 


int carpassing_half = 0;
int master_slaveA_carwait = 0;    ///현재 A에서 기다리고 있는 차량 대수
int master_slaveA_carin = 0;      ///현재 A에서 출발한 차량 대수
int master_slaveB_carwait = 0;    ///현재 B에서 기다리고 있는 차량 대수
int master_slaveB_carin = 0;      ///현재 B에서 출발한 차량 대수
unsigned int master_car_passing_count = 0;  
char master_slaveA_inputval = 0;  ///slaveA 에서 읽은 변수
char master_slaveB_inputval = 0;  ///slave

int prevstate = 1;
int num = 0;
int button_state;
int time_value =0;
int time_output = 0;
char time_sending_place = 0;


void setup() {
  ///////////////// 포트 설정 및 serial 설정////////////
  Serial.begin(9600);
  slaveA_port.begin(9600);
  pinMode(button_pin,INPUT);

  //////////////기본 설정 /////////////////////////////
  slaveA_port.print(master_traffic_Red);
  Serial.print(master_traffic_Red);
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  


////중앙 버튼 클릭 카운트 변수-> 2번 눌려야 한번으로 인식함/////
}

void count_init(){
  master_slaveA_carwait = 0;
  master_slaveA_carin = 0;
  master_slaveB_carwait = 0;
  master_slaveB_carin = 0;
  master_car_passing_count = 0;
  carpassing_half = 0;
  prevstate = 1;
  num = 0;
  lcd.clear();

}

void traffic_light_control(char input_value, char place){
  if(place == 'B'){
    switch(input_value){
      
      case slave_sign_wait :
        master_slaveB_carwait++;
        if( master_slaveB_carin == 0){
          if(master_slaveA_carwait == 0){
            if(master_slaveA_carin == 0){
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Green);
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red);
            }
          }else{
            if(master_slaveA_carin == 0){
              slaveA_port.print(master_traffic_Green);
              Serial.print(master_traffic_Red);
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red);
            }
            
            // A :green , B : Red//
          }
        }
        else{ 
          if(master_slaveA_carwait ==0){
            slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Green);    
          }
          else{
            slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Red);  
          }
        }
      
        break;
      
      case slave_sign_in :
        master_slaveB_carwait--;
        master_slaveB_carin++;
        if(master_slaveB_carin >= 3){
          if(master_slaveB_carwait == 0){
              if(master_slaveA_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);    
              }
              else{
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);  
              }
            }
            else{
              if(master_slaveA_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);              
              }
              else{
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red); 
              }
            }
        }
        else{
          if(master_slaveB_carwait==0){
              if(master_slaveA_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);    
              }
              else{
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);  
              }
            }
            else{
              if(master_slaveA_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Green);              
              }
              else{
                slaveA_port.print(master_traffic_Green);
                Serial.print(master_traffic_Red); 
              }
            }
        }
        
        break;
      case slave_sign_out :
        master_slaveA_carin--;
        if(master_slaveA_carin == 0){
          if(master_slaveA_carwait == 0){
            if(master_slaveB_carwait == 0){
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red); 
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Green); 
            }
          }
          else{
            if(master_slaveB_carwait == 0){
              slaveA_port.print(master_traffic_Green);
              Serial.print(master_traffic_Red);
            }
            else{
              slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Green);
            }  
          }
        }
        else{
          if(master_slaveA_carwait == 0){
            if(master_slaveB_carwait == 0){
              slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Red);
            }
            else{
              slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Red);
            }
            
            }
            else{
              if(master_slaveB_carwait == 0){
              slaveA_port.print(master_traffic_Green);
            Serial.print(master_traffic_Red);
            }
            else{
              slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Red);
            }
                    
          }
        }
        break;

      case slave_sign_passive :
        Serial.print(master_traffic_Yellow);
        slaveA_port.print(master_traffic_Yellow);
        count_init();
        button_pin = 5;
        break;
      case slave_sign_auto :
        Serial.print(master_traffic_Red);
        slaveA_port.print(master_traffic_Red);
        count_init();
        button_pin = 4;
        break;
      default :
        break;
    }
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if(place == 'A'){
    switch(input_value){
     case slave_sign_wait :
         master_slaveA_carwait++;
        if( master_slaveA_carin == 0){
          if(master_slaveB_carwait == 0){
            if(master_slaveB_carin == 0){
              slaveA_port.print(master_traffic_Green);
              Serial.print(master_traffic_Red);
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red);
            }
          }else{
            if(master_slaveB_carin ==0){
             slaveA_port.print(master_traffic_Red);
             Serial.print(master_traffic_Green); 
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red);
            }
            
            // A :green , B : Red//
          }
        }
        else{ 
          if(master_slaveB_carwait ==0){
            slaveA_port.print(master_traffic_Green);
            Serial.print(master_traffic_Red);    
          }
          else{
            slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Red);  
          }
        }
      
        break;
      
      case slave_sign_in :
        master_slaveA_carwait--;
        master_slaveA_carin++;
        if(master_slaveA_carin >= 3){
          if(master_slaveA_carwait == 0){
              if(master_slaveB_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);    
              }
              else{
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);  
              }
            }
            else{
              if(master_slaveA_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);              
              }
              else{
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red); 
              }
            }
        }
        else{
          if(master_slaveA_carwait==0){
              if(master_slaveB_carwait == 0){
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);    
              }
              else{
                slaveA_port.print(master_traffic_Red);
                Serial.print(master_traffic_Red);  
              }
            }
            else{
              if(master_slaveB_carwait == 0){
                slaveA_port.print(master_traffic_Green);
                Serial.print(master_traffic_Red);              
              }
              else{
                slaveA_port.print(master_traffic_Green);
                Serial.print(master_traffic_Red); 
              }
            }
        }
        
        break;

      case slave_sign_out :
        master_slaveB_carin--;
        if(master_slaveB_carin == 0){
          if(master_slaveB_carwait == 0){
            if(master_slaveA_carwait == 0){
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red); 
            }
            else{
              slaveA_port.print(master_traffic_Green);
              Serial.print(master_traffic_Red); 
            }
          }
          else{
            if(master_slaveA_carwait == 0){
              slaveA_port.print(master_traffic_Red);
            Serial.print(master_traffic_Red);
            }
            else{
              slaveA_port.print(master_traffic_Green);
            Serial.print(master_traffic_Red);
            }
                   
          }
        }
        else{
          if(master_slaveB_carwait == 0){
            if(master_slaveA_carwait == 0){
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red); 
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Red); 
            }
            }
            else{
              if(master_slaveA_carwait == 0){
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Green); 
            }
            else{
              slaveA_port.print(master_traffic_Red);
              Serial.print(master_traffic_Green); 
            }        
          }
        }
        break;

      case slave_sign_passive:

        slaveA_port.print(master_traffic_Yellow);
        Serial.print(master_traffic_Yellow);
        count_init();
        button_pin = 5;
        break;
      case slave_sign_auto:

        slaveA_port.print(master_traffic_Red);
        Serial.print(master_traffic_Red);
        count_init();
        button_pin = 4;
        break;
      default :
        break;
    }
  }
}

void loop() {
  ////중앙 버튼 파악//////
  button_state = digitalRead(button_pin);
  if(button_state != prevstate){
    if(button_state == 0){
      if(num == 0){
        num++;
      }
      else{
        carpassing_half++;
 //       Serial.print(carpassing_half);
      }
    }
    prevstate = button_state;
  }

  //////////////////////////////

  if(slaveA_port.available()>0){
    master_slaveA_inputval = slaveA_port.read();
    traffic_light_control(master_slaveA_inputval,'A');
    slaveA_port.print(String(master_slaveA_carin+master_slaveB_carin,DEC));
    slaveA_port.print('a');
    slaveA_port.print(String(master_slaveB_carwait,DEC));
    Serial.print(String(master_slaveA_carin+master_slaveB_carin,DEC));
    Serial.print('a');
    Serial.print(String(master_slaveA_carwait,DEC));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((int) master_slaveB_carwait);
    lcd.setCursor(5,0);
    lcd.print((int) master_slaveB_carin);
    lcd.setCursor(0,1);
    lcd.print((int) master_slaveA_carwait);
    lcd.setCursor(5,1);
    lcd.print((int) master_slaveA_carin);
 /* Serial.print("A에서 기다리는 차량 : ");
    Serial.print(master_slaveB_carwait);
    Serial.print(" 대\n");
    Serial.print("A에서 출발한 차량 : ");
    Serial.print(master_slaveB_carin);
    Serial.print(" 대\n");
    Serial.print("통과 중인 차량 : ");
    Serial.print(master_car_passing_count);
    Serial.print(" 대\n");
    Serial.print("절반이상 통과한 차량 : ");
    Serial.print(carpassing_half);
    Serial.print(" 대\n");
    Serial.print("-----------------------------------------\n");*/
  }
  
  if(Serial.available()>0){
    master_slaveB_inputval = Serial.read();
    traffic_light_control(master_slaveB_inputval,'B');
    slaveA_port.print(String(master_slaveA_carin+master_slaveB_carin,DEC));
    slaveA_port.print('a');
    slaveA_port.print(String(master_slaveB_carwait,DEC));
    Serial.print(String(master_slaveA_carin+master_slaveB_carin,DEC));
    Serial.print('a');
    Serial.print(String(master_slaveA_carwait,DEC));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print((int) master_slaveB_carwait);
    lcd.setCursor(5,0);
    lcd.print((int) master_slaveB_carin);
    lcd.setCursor(0,1);
    lcd.print((int) master_slaveA_carwait);
    lcd.setCursor(5,1);
    lcd.print((int) master_slaveA_carin);
/*    Serial.print("B에서 기다리는 차량 : ");
    Serial.print(master_slaveB_carwait);
    Serial.print(" 대\n");
    Serial.print("B에서 출발한 차량 : ");
    Serial.print(master_slaveB_carin);
    Serial.print(" 대\n");
    Serial.print("통과 중인 차량 : ");
    Serial.print(master_car_passing_count);
    Serial.print(" 대\n");
    Serial.print("절반이상 통과한 차량 : ");
    Serial.print(carpassing_half);
    Serial.print(" 대\n");
    Serial.print("-----------------------------------------\n");*/
  }
  
  
}
