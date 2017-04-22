#include <Regexp.h>
//Commad from console

#define START_CMD_CHAR '*'
#define END_CMD_CHAR '#'
#define DIV_CMD_CHAR '|'
#define CMD_DIGITALWRITE 10
#define CMD_ANALOGWRITE 11
#define CMD_TEXT 12
#define CMD_READ_ARDUDROID 13
#define MAX_COMMAND 20  // max command number code. used for error checking.
#define MIN_COMMAND 10  // minimum command number code. used for error checking. 
#define IN_STRING_LENGHT 40
#define MAX_ANALOGWRITE 255
#define PIN_HIGH 3
#define PIN_LOW 2
String inText;


//For voice commands
int light = 2;
int FAN = 3;
int TV = 4;
int lightWifi = 5;
int FANWifi = 6;
int TVWifi = 7;
int soundDetectedPin = 8; // Use Pin 8 as our Input
int soundModeOnPin = 13;

//Sound controls
int soundDetectedVal = HIGH; // This is where we record our Sound Measurement
boolean bAlarm = false;
unsigned long lastSoundDetectTime; // Record the time that we measured a sound
int soundAlarmTime = 500; // Number of milli seconds to keep the sound alarm high
boolean triggerRelay = false;
int clapcount = 0;
unsigned long timeStart;
unsigned long timeEnd;
unsigned long waitForClapToHappen;
int noOfClapsCommands = 3;//No of claps required to consider it a command
int secondsThreshhold = 10;
int delayBeforeNextClap = 2000;
//Sound controls


int TVWifiLastStatus = LOW;
int FANWifiLastStatus = LOW;
int lightWifiLastStatus = LOW;

boolean tvOn = false;
boolean fanOn = false;
boolean lightOn = false;
unsigned long OneHour = 3600000UL;
unsigned long OneMin = 60000UL; 
unsigned long fanTimeOut = 0UL;
unsigned long lightTimeOut = 0UL;

 int getTime(String cmd){
   MatchState ms;
   unsigned int index = 0;
   char buf [50];
   char command[50];
   cmd.toCharArray(command, 50) ;
   ms.Target (command);
   char result = ms.Match ("(%d+)", index);
   
if (result == REGEXP_MATCHED){
     String timerStr = ms.GetMatch (buf);
     int timer = timerStr.toInt() ;
     return timer;
    }  
    else {
      return 0;
    }
      
 }
void setup()
{
  Serial.begin(9600);
  pinMode(TV,OUTPUT);
  digitalWrite(TV,LOW);
  pinMode(FAN,OUTPUT);
  digitalWrite(FAN,LOW);
  pinMode(light,OUTPUT);
  digitalWrite(light,LOW);
  pinMode(TVWifi,INPUT);
  pinMode(lightWifi,INPUT);
  pinMode(FANWifi,INPUT);
  pinMode(soundDetectedPin,INPUT);
  pinMode(soundModeOnPin,OUTPUT);
  
  
  Serial.flush();   
}

void toggleFan(){
   fanOn = !fanOn;
   if (fanOn){
   digitalWrite(FAN,HIGH); 
  }else{
    digitalWrite(FAN,LOW);
  }
}
void toggleTV(){
  tvOn = !tvOn;
  if (tvOn){
   digitalWrite(TV,HIGH); 
  }else{
    digitalWrite(TV,LOW);
  }
  
}
void toggleLight(){
  lightOn = !lightOn;
  if(lightOn){
    digitalWrite(light,HIGH);
  }else{
    digitalWrite(light,LOW);
  }
    
}

void getStatus(){
  String stat = "";
  if (fanTimeOut > 0){//Fan time set
    
    if (fanOn){
      stat += " FAN off:";
    }else {
      stat += " FAN on:" ; 
    }
    stat += ( (fanTimeOut - millis()) /OneMin);
  }
  if (lightTimeOut > 0){//light time set
    
    if (lightOn){
      stat += " Light off:";
    }else {
      stat += " Light on:" ; 
    }
    stat += ( (lightTimeOut - millis()) /OneMin);
  }
  Serial.println( stat);
}
void checkClap(){
  
  digitalWrite (soundModeOnPin,HIGH);//Indicator that it is  listing to sound
  Serial.println( "Waiting for sound to happen");
  waitForClapToHappen = millis();
   while (( (millis() - waitForClapToHappen) < 1000 * secondsThreshhold) ){//wait for clap to happen
          soundDetectedVal = digitalRead (soundDetectedPin) ; // read the sound 
          triggerRelay = false;
          if (soundDetectedVal == HIGH){
            timeStart =  millis();
            Serial.println("Sound detected");
             clapcount = 1;
             Serial.println(clapcount);
             digitalWrite (soundModeOnPin,LOW);//Indicator that it is not listing to sound
             delay(delayBeforeNextClap);// Wait for some time so that a clap is not counted multiple times
              digitalWrite (soundModeOnPin,HIGH);//Indicator that it is  listing to sound
            while (( (millis() - timeStart) < 1000 * secondsThreshhold) ){//if clap count is 3 or 15 seonds is elapsed which ever is less
              soundDetectedVal = digitalRead (soundDetectedPin) ; //read the sound 
              if (soundDetectedVal == HIGH){
               clapcount++;
               Serial.println(clapcount);
               if (clapcount < noOfClapsCommands ) {
                 digitalWrite (soundModeOnPin,LOW);//Indicator that it is not listing to sound
                delay(delayBeforeNextClap);// Wait for some time so that a clap is not counted multiple times, also to reduce environment noice
                 digitalWrite (soundModeOnPin,HIGH);//Indicator that it is  listing to sound
               }else {
                break;
               }
                
              }
            }
            
            if (clapcount >= noOfClapsCommands){
              Serial.println(" Will execute this order and wait for new  ");
                triggerRelay = true;
              }else{
                Serial.println(" Nothing to execute  ");
                triggerRelay = false;
              }
            
            
          }
          if (triggerRelay){
            toggleLight();
          }
   }
   Serial.println( "Wait over");
   digitalWrite(soundModeOnPin,LOW);//will not listen sound command
}
void checkWifi(){
  if (digitalRead(lightWifi) != lightWifiLastStatus) {//There is a change in WIFI status since its last reported status
    lightWifiLastStatus = digitalRead(lightWifi);
    digitalWrite(light,lightWifiLastStatus); 
    if (lightWifiLastStatus == HIGH){
       lightOn = true;
    }else{
      lightOn = false;
    }
   
  }
  if (digitalRead(FANWifi) != FANWifiLastStatus) {//There is a change in WIFI status since its last reported status
    FANWifiLastStatus = digitalRead(FANWifi);
    digitalWrite(FAN,FANWifiLastStatus); 
    if (FANWifiLastStatus == HIGH){
      fanOn = true;
    }else {
      fanOn = false;
    }
     
  }
}
void loop1(){
 checkClap(); 
}
void loop()
{
 //checkWifi();
 checkClap();
  if (fanTimeOut > 0){//Fan time set
     if (millis() > fanTimeOut){
       toggleFan();
       fanTimeOut = 0UL;
     }
  }
   if (lightTimeOut > 0){//Light time set
    if (millis() > lightTimeOut){
      toggleLight();
      lightTimeOut = 0UL;
    }
  }
  delay(1000);
  Serial.flush();
 if(Serial.available())
 {
   String value = Serial.readStringUntil('\n');
   if (value.indexOf(DIV_CMD_CHAR) > 0){//Console mode
      checkConsoleCommands(value);
   }else {//Voice mode
       executeVoiceCommand(value);
     }
   }
   
}

void executeVoiceCommand(String value){
 value.toLowerCase();
 if ( value.startsWith("*timer")){
  getStatus();
 }
      Serial.println(value);
       if(value.startsWith("*fan ") || value.startsWith("*phone ") || value.startsWith("*find ") || value.startsWith("*punk ") || value.startsWith("*uncle ") 
         || value.startsWith("*i'm caught ") || value.startsWith("*f*** ") || value.startsWith("*i'm cut ")  || value.startsWith("*phone call ") || value.startsWith("*sound ") || value.startsWith("*send ")) {
        if (value.endsWith(" off#")){
          fanOn = false;
          digitalWrite(FAN,LOW);
        }else if (value.indexOf("minute") > 0){
         int minutes = getTime(value);
         Serial.println( "FAN will be  " ); 
         if (fanOn){
          Serial.print( "off in minutes = ");
         }else {
          Serial.print( " on in minutes = ");
         }
         Serial.print( minutes);
         fanTimeOut  = OneMin * minutes;
       }else if (value.indexOf("hour") > 0){
           int hour = getTime(value);
          Serial.println( "FAN will be  " ); 
         if (fanOn){
          Serial.print( "off in hours = ");
         }else {
          Serial.print( " on in hours = ");
         }
         Serial.print( hour);
         fanTimeOut  = OneHour * hour;
        }else{
           fanOn = true;
          digitalWrite(FAN,HIGH);
        }
        
         
       }
       if(value.startsWith("*tv ") || value.startsWith("*bb ") )  {
        if (value.endsWith(" off#")){
          digitalWrite(TV,LOW);
          tvOn = false;
        }else{
          digitalWrite(TV,HIGH);
          tvOn = true;
        } 
       
       }
    
       if(value.startsWith("*light ")  )  {
        if (value.endsWith(" off#")){
          digitalWrite(light,LOW);
          lightOn = false;
        }else if (value.indexOf("minute") > 0){
         int minutes = getTime(value);
           Serial.println( "Light will be  " ); 
         if (lightOn){
          Serial.print( "off in minutes = ");
         }else {
          Serial.print( " on in minutes = ");
         }
         Serial.print( minutes);
         lightTimeOut  = OneMin * minutes;
           
        }else if (value.indexOf("hour") > 0){
          int hour = getTime(value);
           Serial.println( "Light will be  " ); 
         if (lightOn){
          Serial.print( "off in hours = ");
         }else {
          Serial.print( " on in hours = ");
         }
         Serial.print( hour);
         lightTimeOut  = OneHour * hour;
        }else{
          digitalWrite(light,HIGH);
          lightOn = true;
        }
         
       }
}


void checkConsoleCommands(String value)
{
 
  value.replace("#", "");
  value.replace("*", "");
 
  
  int ard_command = 0;
  int pin_num = 0;
  int pin_value = 0;



  // parse incoming command type
 ard_command = value.substring(0,2).toInt(); // read the command
  
  // parse incoming pin# and value  
  pin_num = value.substring(3,5).toInt(); // read the pin
  if (ard_command == CMD_DIGITALWRITE ){
    pin_value = value.substring(6).toInt();  // read the value
  }else if (ard_command == CMD_TEXT  ){
    pin_value = value.substring(6,8).toInt();  // read the value\
    inText =""; //clears variable for new input   
    inText =  value.substring(9);
  }else if (ard_command == CMD_ANALOGWRITE  ){
    pin_value = value.substring(6,value.indexOf(DIV_CMD_CHAR,6)).toInt();  // read the value
  }


  // 1) GET TEXT COMMAND FROM ARDUDROID
  if (ard_command == CMD_TEXT){ 
    
    inText.trim(); 
     executeVoiceCommand("*"+inText+"#");     
  }

  // 2) GET digitalWrite DATA FROM ARDUDROID
  if (ard_command == CMD_DIGITALWRITE){  
    if (pin_value == PIN_LOW) pin_value = LOW;
    else if (pin_value == PIN_HIGH) pin_value = HIGH;
    else return; // error in pin value. return. 
    set_digitalwrite( pin_num,  pin_value);  // Uncomment this function if you wish to use 
    return;  // return from start of loop()
  }

  // 3) GET analogWrite DATA FROM ARDUDROID
  if (ard_command == CMD_ANALOGWRITE) {  
    analogWrite(  pin_num, pin_value ); 
    // add your code here
    return;  // Done. return to loop();
  }

  // 4) SEND DATA TO ARDUDROID
  if (ard_command == CMD_READ_ARDUDROID) { 
    // char send_to_android[] = "Place your text here." ;
    // Serial.println(send_to_android);   // Example: Sending text
    Serial.print(" Analog 0 = "); 
    Serial.println(analogRead(A0));  // Example: Read and send Analog pin value to Arduino
    return;  // Done. return to loop();
  }
}

// 2a) select the requested pin# for DigitalWrite action
void set_digitalwrite(int pin_num, int pin_value)
{
  switch (pin_num) {
  case 13:
    pinMode(13, OUTPUT);
    digitalWrite(13, pin_value);  
    // add your code here      
    break;
  case 12:
    pinMode(12, OUTPUT);
    digitalWrite(12, pin_value);   
    // add your code here       
    break;
  case 11:
    pinMode(11, OUTPUT);
    digitalWrite(11, pin_value);         
    // add your code here 
    break;
  case 10:
    pinMode(10, OUTPUT);
    digitalWrite(10, pin_value);         
    // add your code here 
    break;
  case 9:
    pinMode(9, OUTPUT);
    digitalWrite(9, pin_value);         
    // add your code here 
    break;
  case 8:
    pinMode(8, OUTPUT);
    digitalWrite(8, pin_value);         
    // add your code here 
    break;
  case 7:
    pinMode(7, OUTPUT);
    digitalWrite(7, pin_value);         
    // add your code here 
    break;
  case 6:
    pinMode(6, OUTPUT);
    digitalWrite(6, pin_value);         
    // add your code here 
    break;
  case 5:
    pinMode(5, OUTPUT);
    digitalWrite(5, pin_value); 
    // add your code here       
    break;
  case 4:
    pinMode(4, OUTPUT);
    digitalWrite(4, pin_value);         
    // add your code here 
    break;
  case 3:
    pinMode(3, OUTPUT);
    digitalWrite(3, pin_value); 
    if (pin_value == HIGH){
      fanOn = true;
    }else{
      fanOn = false;
    }        
    break;
  case 2:
    pinMode(2, OUTPUT);
    digitalWrite(2, pin_value);  
    if (pin_value == HIGH){
      lightOn = true;
    }else{
      lightOn = false;
    }
        
    break;      
    // default: 
    // if nothing else matches, do the default
    // default is optional
  } 
}
