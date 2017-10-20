//SW project
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>    //library for lcd
#include <Keypad.h>                  //library for keypad(button)

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

const int numRows= 3;
const int numCols= 2;
const int max_user = 15;
const int max_list = 10;

//definition of keypad
char hexaKeys[numRows][numCols] = {
  { 'U', 'S'},
  { 'D', 'M'},
  { 'R', '0'}
};

byte rowPins[numRows] = {2, 3, 4};   //pin num of row
byte colPins[numCols] = {6, 7};      //pin num of col

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, numRows, numCols); //create keypad instance

struct USER{
  String rank;      //rank
  String memname;   //name
  int current_list; //current list pos
};

USER users[max_user];  //users ex. (계급, 이름 )
int user_num;           //total user num
int list_num;           //total list num
char list[max_list][15];  //status list

int currentPos = 0;   
char customKey = 0;
int listpos = 0;        
bool check_SR = false;    //check whether if Select button pressed or Reset button pressed
bool check_first = false; //check whether if Select button pressed first.
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:

  lcd.begin();     //initialize the led 
  lcd.clear();     //clear lcd
  lcd.setCursor(5,0);
  lcd.print("!P 1S");   //1 platoon 1 squad

  //
  user_num = 2;
  list_num = 2;
  users[0].rank = "private";
  users[0].memname = "woosuk";
  users[0].current_list = 0;
  users[1].rank = "corporal";
  users[1].memname = "hosung";
  users[1].current_list = 0;

  strcpy(list[0], "not");
  strcpy(list[1], "cyber");
  //
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while(1){
    if(Serial.available()){   //data receive
      lcd.clear();
      receive_data();
      lcd.setCursor(5,0);
      lcd.print("!P 1S");   //1 platoon 1 squad
    }
    else{
      customKey = customKeypad.getKey();

      if(customKey){
        switch (customKey){
          case 'U':     //pressed Up key
            check_first = true;
            if(currentPos == 0){      //Point first userMember
              currentPos = user_num - 1;
            }
            else{
              currentPos--;
            }
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(users[currentPos].rank + " " + users[currentPos].memname);
            lcd.setCursor(0,1);
            lcd.print(list[users[currentPos].current_list]);
          break;
          
          case 'D':     //pressed Down key
            check_first = true;
            if(currentPos == user_num-1){      //Point last userMember
              currentPos = 0;
            }
            else{
              currentPos++;
            }
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(users[currentPos].rank + " " + users[currentPos].memname);
            lcd.setCursor(0,1);
            lcd.print(list[users[currentPos].current_list]);
          break;

          case 'S':     //pressed Select Key
            if(check_first == false) break;
            
            listpos = users[currentPos].current_list;
            check_SR = false;
            
            while(1){
               if(check_SR){
                lcd.noBlink();
                if(!Serial.available()){     //if not server send data to this device at the same time
                  sendUpdate();             //send updated data
                }
                
                break;
               }
               
               lcd.blink();
               customKey = customKeypad.getKey();
  
               switch(customKey){
                case 'U':
                  if(listpos == 0){
                    listpos = list_num - 1;
                  }
                  else{
                    listpos--;
                  }
                  lcd.setCursor(0,1);
                  for(int i=0; i<16; i++){
                    lcd.print(" ");
                  }
                  lcd.setCursor(0,1);
                  lcd.print(list[listpos]);
                break;
  
                case 'D':
                 if(listpos == (list_num-1)){
                    listpos = 0;
                  }
                  else{
                    listpos++;
                  }
                  lcd.setCursor(0,1);
                  for(int i=0; i<16; i++){
                    lcd.print(" ");
                  }
                  lcd.setCursor(0,1);
                  lcd.print(list[listpos]);
              break;

              case 'S':
                users[currentPos].current_list = listpos;
                check_SR = true;
              break;

              case 'R':
                listpos = 0;
                users[currentPos].current_list = 0;
                lcd.setCursor(0,1);
                for(int i=0; i<16; i++){
                    lcd.print(" ");
                }
                lcd.setCursor(0,1);
                lcd.print(list[0]);
                check_SR = true;
              break;
              
              default:
              break;
             }
          }
          break;

          case 'M':     //pressed Mode Key
            lcd.clear();
            lcd.setCursor(5,0);
            lcd.print("1P 1S");
            lcd.setCursor(5,1);
            lcd.print(cal_ps());

            while(1){
              customKey = customKeypad.getKey();
              if(customKey == 'M') break;
            }
            lcd.clear();
            lcd.setCursor(5,0);
            lcd.print("1P 1S");
            check_first = false;
          break;

          default:
          break;
        }
      }
    }
  }
}

//calculate and return personnel situation
String cal_ps(){
  int num_except = 0;   //except member num
  int num_present = 0;  //present member num

  for(int i=0; i<user_num; i++){
    if((strcmp(list[users[i].current_list], "not")) != 0){   //if a status list is not "not" 
      num_except++;
    } 
  }

  num_present = user_num - num_except;

  //make char array(print format)
  char temp[6];
  temp[0] = user_num + '0';
  temp[1] = '/';
  temp[2] = num_except + '0';
  temp[3] = '/';
  temp[4] = num_present + '0';
  temp[5] = 0;  // 6th array element is a null terminator;

  String temp_s(temp);
  
  return temp_s; 
}


//Message Format = "#/total_usernum/rank/name/rank..." or "$/list_num/list1/list2..."
//p = private, pf = private first class, c = corporal, s = sergent
//Message ex. "#/3/p/Hong/pf/Kim/c/Nam" or "$/3/not/workout/furlough"
//recive_data and split string and update
void receive_data(){
  int nCount = 0;
  int nGetIndex = 0 ;
  String sData;
  
  char type = Serial.peek(); // if type is "#" userlist Message or not status list Message
  
  sData = Serial.readString();  //read text
  
  //temp rank, name, status string for split
  String rTemp = "";
  String nTemp = "";
  String sTemp = "";
  //copy original text for split
  String sCopy = sData;

  if(strcmp(type, '#') == 0){               // userlist Message
    //start split 
    
    nGetIndex = sCopy.indexOf("/");         // "/" is separator
    sCopy = sCopy.substring(nGetIndex + 1); //cut string ("#/")
    
    user_num = sCopy.charAt(0) - '0';       //get user num
    nGetIndex = sCopy.indexOf("/");
    sCopy = sCopy.substring(nGetIndex + 1); //cut string("num/") ex. ("9/")

    memset(&users, 0, sizeof(USER)*max_user);  //initialize user list
    
    for(int i=0; i<user_num-1; i++){
      nGetIndex = sCopy.indexOf("/");
      rTemp = sCopy.substring(0, nGetIndex);    //get rank
      sCopy = sCopy.substring(nGetIndex + 1);

      nGetIndex = sCopy.indexOf("/");
      nTemp = sCopy.substring(0, nGetIndex);    //get name
      sCopy = sCopy.substring(nGetIndex + 1);

      users[i].rank = rTemp;
      users[i].memname = nTemp;
    }
    nGetIndex = sCopy.indexOf("/");
    rTemp = sCopy.substring(0, nGetIndex);      //get rank of last user
    sCopy = sCopy.substring(nGetIndex + 1);
    nTemp = sCopy;

    users[user_num-1].rank = rTemp;
    users[user_num-1].memname = nTemp;
    //end update
  }
  else if(strcmp(type, '$') == 0){  //status list Message
    char buf[15];     //temp buffer
    
    //start split 
    nGetIndex = sCopy.indexOf("/");         // "/" is separator
    sCopy = sCopy.substring(nGetIndex + 1); //cut string ("$/")
   
    list_num = sCopy.charAt(0) - '0';       //get list num
    nGetIndex = sCopy.indexOf("/");
    sCopy = sCopy.substring(nGetIndex + 1); //cut string("num/") ex. ("9/")
   
    //initialize status list
    for(int i=0; i<max_list; i++){
      memset(list[i], 0, sizeof(char)*15);
    }

    for(int i=0; i<list_num-1; i++){
       nGetIndex = sCopy.indexOf("/");
       sTemp = sCopy.substring(0, nGetIndex);
       sCopy = sCopy.substring(nGetIndex + 1);
       sTemp.toCharArray(buf, 15);
       strcpy(list[i], buf);
    }
    sCopy.toCharArray(buf, 15);
    strcpy(list[list_num-1], buf);    //update last status list
  } 
  else{ // error
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Message Format");
    lcd.setCursor(7,1);
    lcd.print("Error!");
    return;
  }
}

//send updated state
//send Message Format "specific_num/user_num/except_num/present_num" ex. when this device is of 1P1S, Message is "11/9/1/8"
void sendUpdate(){
  String message = "11/";
  String temp = cal_ps();
  
  message.concat(temp);
  
  char temp_message[message.length()+1];
  message.toCharArray(temp_message, message.length()+1); 

  Serial.write(temp_message);
}


