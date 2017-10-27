//=======================================================================
// 프로젝트명 : 사물인터넷을 이용한 체력측정 및 관리 도우미
// 작  성  자 : 조명진 , 조민규 
// 작성일자   : 2017년 10월16일과 20일 
// 제작목적   : 군 체력 검정시 기계에 의한 기준을 제시하여 공정한 측정이 되도록 한다.
//              평상시에도 활용하여 개인별 데이타를 축적하고 이것을 분석하여
//              체력관리 미흡자가 측정당일날 문제가 발생하지 않도록 미리 관리한다.
// 설     명  : 군 검정시 활용하여 누구나 공정한 기준에서 측정하게 한다.
//              ------ 이하는 제한된 자원으로 현시점에 구현하지는 못하였다.
//              평상시에 체력관리도구로 이용하여 기록을 데이타베이스에 축적한다.
//              데이타베이스를 활용하여 체력관리가 잘 되지 않는 인원을 주기적으로 
//              검색하여 독려한다. 최소한 연습횟수를 지정하여 충족하지 못하면 체력검정
//              자격을 주지 않는다. 관리없이  갑자스러운 측정에 따른 몸의 과부하에 따른 
//              사망사건을 사전에 방지할수 있다. 
//              또한 데이타베이스를 활용하여 카카오 게임같은 것을 개발하여 보급하면 좋을것
//              같다. 게임처럼 검정종목에 결과를 전시하고 ,톡으로 보내어 주변 지인의 참여알림등으로
//              평상시에도 참여가 확대할수 있을것 같다.
//=======================================================================

#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x3f, 16, 2);  //한참삽질... lcd주소 초기화를 못시켜주셔.....0x3f로 되어있지 않은 경우가 많음

int TRIGGER_OUT = 2; 
int ECHO_IN = 3; 
int SND=12;
int cnt=0;
int sec;
int mode=0;
int m=0;
int hh,mm,ss;
int i,j,k=0;
int speakerpin=13;
boolean up_flag=false;
boolean down_flag=false; 

void setup()
{
   Serial.begin(9600); 
   pinMode(7, INPUT); 
   pinMode(8, INPUT); 
   pinMode(TRIGGER_OUT, OUTPUT); 
   pinMode(10, OUTPUT); 
   pinMode(ECHO_IN, INPUT);    
   pinMode(SND,OUTPUT);
 }
 
void loop()
{
    menu();
}
 
void menu()
{
   String txtsc="1.Sit UP 2.Push UP 3.Run";  //16
   String txtsc2="UP 3)Run";
   lcd.begin();
   lcd.backlight();
   lcd.setCursor(0,0); //위
   lcd.println("=CHECK YOUR PWR=");
   lcd.setCursor(0,1); //아래
   lcd.println(txtsc);
  
   //=================================================
   ///문자스크롤 을 위한 변수
   int stringStart, stringStop = 0; 
   int scrollCursor = 16;
   //=================================================
   int jjj=0;
   String kkk="";
   while(m==0)
   {   
      //=================================================
     //글자수가 많아서 스크롤을 해야함.
     //아래줄의 내용을 왼쪽으로 shift
     //아래쪽을 지우고 쓰기때문에 깜박임은 어쩔수 없나?
      lcd.setCursor(scrollCursor, 1);  //lcd아래쪽의 시작점 설정
      lcd.print(txtsc.substring(stringStart,stringStop));
      delay(300);
      lcd.clear();
      lcd.setCursor(0,0); //위
      lcd.println("=CHECK YOUR PWR=");
      if(stringStart == 0 && scrollCursor > 0)
      {
        scrollCursor--;
        stringStop++;
      }
        else if (stringStart == stringStop)
        {
          stringStart = stringStop = 0;
          scrollCursor = 16;
        } 
        else if (stringStop == txtsc.length() && scrollCursor == 0)
        {
          stringStart++;
        }
          else
          {
            stringStart++;
            stringStop++;
          }
     
      if (scrollCursor>15)
      {
        stringStart = stringStop = 0;
        scrollCursor = 16;
      }
   //======================================================

   //======================================================
   //메뉴선택 : 버튼 3개중 하나를 입력받아 해당 메뉴로 분기
   //======================================================
     int m= digitalRead(7);
     delay("1000");
     if (m==HIGH) //7번버튼을 먼저검사
     {
        mode=1;
        situp();
     } 
       else
       {
         m= digitalRead(8);
         if (m==HIGH) //8번버튼을 검사
         {
           mode=2;
           pushup(); 
         } 
         m= digitalRead(9);
         if (m==HIGH)   //9번버튼을 검사
         {
           mode=3;
           runn();
         }
       }
   }
}

void situp() 
{   
   sound();
   timer_start();
   lcd.clear();
   lcd.setCursor(0,0); //위줄
   lcd.print("=SIT UP execute=");
   lcd.setCursor(0,1); //아래줄
   lcd.print("COUNT=> ");
   sec=0;
   timer_start();  //타이머 시작 ,체력검정 시간은 2분임
   while(sec<120)
   {           
      check_count();          
      lcd.setCursor(8,1);
      lcd.print(cnt);
      lcd.print(" T= ");
      lcd.print(sec);
        if (sec>3)
        {
          if(digitalRead(7)==HIGH)
          {
            break;
          }
        }   
   }
       
   lcd.clear();    
   lcd.setCursor(2,0); //위
   lcd.print("=SIT UP END=");
   lcd.setCursor(2,1); //아래
   lcd.print("COUNT : ");
   lcd.print(cnt);
   cnt = 0;
   finish();
   menu();
}

void pushup() 
{
   sound();
   lcd.clear();
   lcd.setCursor(0,0); //위
   lcd.print("=PUSH UP EXE=");
   lcd.setCursor(0,1); //아래
   lcd.print("count=> ");
   sec=0;
   timer_start();

   while(sec<120)
   {
      check_count();
      lcd.setCursor(8,1);
      lcd.print(cnt);
      lcd.print(" T= ");
      lcd.print(sec);
        if (sec>3)
        {
          if(digitalRead(8)==HIGH)
          {
             break;
          }
        }   
   }
   
   lcd.clear();    
   lcd.setCursor(2,0); //위
   lcd.print("=PUSH UP END=");
   lcd.setCursor(2,1); //아래
   lcd.print("COUNT : ");
   lcd.print(cnt);
   cnt = 0;
   finish();
   menu();
}

void runn() 
{
   sound();
   lcd.clear();
   lcd.setCursor(0,0); //위
   lcd.print("=RUN EXE=");
   lcd.setCursor(0,1); //아래
   lcd.print("TIME=> ");
   sec=0;
   timer_start();
   
   while(1)
   {       
      lcd.setCursor(10,1);
        if (sec >= 60 && sec < 3600)
        {
           mm=sec/60;
           ss=sec%60;
           lcd.setCursor(10,1);
           lcd.print (mm);   
           lcd.print (":");
             if (ss < 10)
             {
               lcd.print(0);
               lcd.print (ss);
             }
               else
                 lcd.print(ss);
        }
          else if (sec >= 3600)
          {
             hh=sec/3600;
             mm=(sec%3600)/60;
             ss=(sec%3600)%60;
             lcd.setCursor(10,1);
             lcd.print (hh);   
             lcd.print (":");
               if (mm < 10)
               {
                  lcd.print(0);
                  lcd.print (mm);
               }
                 else
                 {
                   lcd.print (mm);  
                   lcd.print (":");
                 }
               if (ss < 10)
               {
                 lcd.print(0);
                 lcd.print (ss);
               }
                 else
                   lcd.print(ss);
          }
          else
          lcd.print(sec);
          if (sec>3)
          {
             if(digitalRead(9)==HIGH)
             {
                    break;
             }
          }   
   }
   lcd.clear();    
   lcd.setCursor(2,0); //위
   lcd.print("=RUN END=");
   lcd.setCursor(2,1); //아래
   lcd.print("TIME : ");
   if (sec >= 60 && sec < 3600)
   {         
      lcd.print (mm);   
      lcd.print (":");
      lcd.print (ss);
   }
      else if (sec >= 3600)
      {    
         lcd.print (hh);   
         lcd.print (":");
         lcd.print (mm);   
         lcd.print (":");
         lcd.print (ss);
      }
         else
         lcd.print(sec);
         
         finish();
         menu();
}


void finish()
{
  tone(speakerpin,1000,200);  //500: 음의 높낮이(주파수), 1000: 음의 지속시간(1초)
  delay(250);
  tone(speakerpin,1000,200);
  delay(250);
  tone(speakerpin,1000,200);
  delay(250); 
  tone(speakerpin,1000,200);
  delay(250); 
  tone(speakerpin,1000,200);
  delay(250); 
  delay(500);
  tone(speakerpin,1000,200);  //500: 음의 높낮이(주파수), 1000: 음의 지속시간(1초)
  delay(250);
  tone(speakerpin,1000,200);
  delay(250);
  tone(speakerpin,1000,200);
  delay(250); 
  tone(speakerpin,1000,200);
  delay(250); 
  tone(speakerpin,1000,200);
  delay(250); 
  delay(500);
  tone(speakerpin,1000,200);  //500: 음의 높낮이(주파수), 1000: 음의 지속시간(1초)
  delay(250);
  tone(speakerpin,1000,200);
  delay(250);
  tone(speakerpin,1000,200);
  delay(250); 
  tone(speakerpin,1000,200);
  delay(250); 
  tone(speakerpin,1000,200);
  delay(250); 
 
}

void timer_start()
{
   Timer1.initialize(1000000); // 타이머 설정
   Timer1.attachInterrupt( timerIsr ); // 타이머 서비스루틴 설정
}
void timerIsr()
{
    // Toggle LED
    sec++;  //타이머 인트럽트 발생(1초단위) 시 sec변수를 증가   
}

void sound()
{
 lcd.clear();
 lcd.setCursor(5,0);
 lcd.print("Ready");
   for(i=3;i>j;i--)
   {    
       digitalWrite(13,HIGH);
       Serial.println("13--> high");
       lcd.setCursor(7,1);
       lcd.print(i);
       delay(1000);
       digitalWrite(13,LOW);
       Serial.println("13--> low");
       delay(1000);
       
    }
    lcd.setCursor(3,1);
    lcd.print("Start !!");
    delay("5000");
}


int check_count()
{ 

  int x=0,y=0; //x = 누운거 , y=일어선거
  long duration, cm; 
  digitalWrite(TRIGGER_OUT, HIGH); 
  delay(10); 
  digitalWrite(TRIGGER_OUT, LOW); 
  duration = pulseIn(ECHO_IN, HIGH); 
  cm = microseondsToCentimeters(duration); 

    
//거리가 0<distance<15이면 아래로 움직인 것이므로 down_flag를 true로 설정
    if ((0<cm) && (cm<15) ){
    down_flag=true;
    up_flag=false;                        //다운을 한후, up을 해줘야 카운트가 올라가게끔!
    }    
//15~20 사이의 값을 넣지 않은이유 : 15근방에서 부르르 떨경우, 운동한 갯수가 올라가므로 제외해 주었다.

 //거리가 20<distance<50이면 위로 움직인 것이므로 up_flag를 true로 설정    
    else if( (20<cm) && (cm<50) ){
    up_flag=true;
    }
 //그밖의 경우 플래그를 변경해 주지 않는다.
    else{      
  }      
//down_flag와 up_flag가 true 라면         
    if( (down_flag==true) && (up_flag==true) ){
    //푸쉬업 카운트 증가,
       cnt++;

        down_flag=false;
        up_flag=false; 
        tone(13,500,100); 
            
    }


} 


// 1/1000 초를 인치(Inch) 거리로 변환하는 함수 
long microsencodsToInches(long microseconds) 
{ 
  // 인치당 73.746초가 소요됨. 음파 속도는 초당 1130 피트를 이동함. 
  // 왕복 거리 이므로 2로 나눔 
  return microseconds / 74 / 2; 
} 

long microseondsToCentimeters(long microseconds) 
{ 
  // 음파 속도는 초당 340 m/s, 1cm당 29 ms이 소요됨 
  // 왕복 거리 이므로 2로 나눔 
  return microseconds / 29 / 2; 

} 

