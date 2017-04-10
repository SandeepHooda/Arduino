/**
  ******************************************************************************
  * @file    vr_sample_multi_cmd.ino
  * @author  JiapengLi
  * @brief   This file provides a demostration on 
              how to implement a multi voice command project (exceed 7 voice command) 
              by using VoiceRecognitionModule
  ******************************************************************************
  * @note:
        voice control led
  ******************************************************************************
  * @section  HISTORY
    
    2013/06/13    Initial version.
  */
  
#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

/**        
  Connection
  Arduino    VoiceRecognitionModule
   2   ------->     TX
   3   ------->     RX
*/
VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.

uint8_t record[7]; // save record
uint8_t buf[64];
int lastCmd = 0;
int timerLED1 = 7;
int timerLED2 = 8;
int led = 6;
int light = 12;
int cfl = 11;
int fan = 10;
int small = 9;

unsigned long fifteenMin = 900000UL; 
int timeInMin = 0;
unsigned long timeOut = 0UL; 

int group = 0;
/*
sigtrain 1 shinchan
sigtrain 10 light
sigtrain 11 cfl
sigtrain 12 fan
sigtrain 13 small

sigtrain 0 sonu
sigtrain 6 light
sigtrain 7 cfl
sigtrain 8 fan
sigtrain 9 small

sigtrain 2 dadi
sigtrain 14 light
sigtrain 15 cfl
sigtrain 16 fan
sigtrain 17 small
 */

#define sonu        (0)
#define shaurya     (1)
#define dadi        (2)
#define kusum       (3)
#define dada        (4)

#define sonu1       (6) 
#define sonu2       (7) 
#define sonu3       (8) 
#define sonu4       (9) 

#define shaurya1       (10) 
#define shaurya2       (11) 
#define shaurya3       (12) 
#define shaurya4       (13) 

#define dadi1       (14) 
#define dadi2       (15) 
#define dadi3       (16) 
#define dadi4       (17) 

boolean dadiLoaded = false;
boolean sonuLoaded = false;
boolean shauryaLoaded = false;
void setup()
{
  /** initialize */
  myVR.begin(9600);
  
  Serial.begin(115200);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nMulti Commands sample");
  
  pinMode(led, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(cfl, OUTPUT);
  pinMode(small, OUTPUT);
  pinMode(timerLED1, OUTPUT);
  pinMode(timerLED2, OUTPUT);

  digitalWrite(light, LOW); //Switch off every thing
  digitalWrite(fan, LOW);
  digitalWrite(cfl, LOW); 
  digitalWrite(small, LOW);
  digitalWrite(timerLED1, LOW); 
  digitalWrite(timerLED2, LOW); 
  
     
    
  if(myVR.clear() == 0){
    Serial.println("Recognizer cleared.");
  }else{
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  record[0] = sonu;
  record[1] = sonu1;
  record[2] = sonu2;
  record[3] = sonu3;
  record[4] = sonu4;
  
  if(myVR.load(record, 5) >= 0){
    printRecord(record, 5);
    Serial.println(F("loaded."));
  }
  
}
void setTimer(){
   timeOut += fifteenMin;
   timeInMin += 15;
   if (timeInMin > 45){//Can't set more that 15 min so reset timer
    resetTimer(); 
   }
    
   if (timeInMin == 15){
    digitalWrite(timerLED1, HIGH); 
    digitalWrite(timerLED2, LOW);
    Serial.println("Timer Set in 15 min "); 
   }
   if (timeInMin == 30){
     digitalWrite(timerLED1, LOW); 
    digitalWrite(timerLED2, HIGH);  
    Serial.println("Timer Set in 30 min "); 
   }
   if (timeInMin == 45){
     digitalWrite(timerLED1, HIGH); 
    digitalWrite(timerLED2, HIGH); 
    Serial.println("Timer Set in 45 min "); 
   }
}
void resetTimer(){
  digitalWrite(timerLED1, LOW); 
  digitalWrite(timerLED2, LOW); 
  timeInMin = 0;
  timeOut = 0UL;
  Serial.println("Timer reset ");
}

void triggerTimeOut() {
  toggle (lastCmd ) ;
  resetTimer(); 
}
void toggle(int pin){
  
         if(digitalRead(pin) == HIGH){
          digitalWrite(pin, LOW);
        }else{
          digitalWrite(pin, HIGH);
        }
}

void understoodCmd(){
  Serial.println("Command OK");
  if(digitalRead(led) == HIGH){
          digitalWrite(led, LOW);
        }else{
          digitalWrite(led, HIGH);
        }
        
}
void loop()
{
  if (timeOut > 0 ){ //Timer Set
    if (millis() > timeOut){
      triggerTimeOut();
    }
  }
  int ret;
  ret = myVR.recognize(buf, 50);
  if(ret>0){
    switch(buf[1]){
      case sonu1: 
      case shaurya1:
      case dadi1:
        lastCmd = light;
        toggle(lastCmd);
        understoodCmd();
        break;

     case sonu2: 
     case shaurya2:
     case dadi2:
      lastCmd = cfl;
      toggle(lastCmd);
      understoodCmd();
       break;

      case sonu3: 
      case shaurya3:
      case dadi3:
        lastCmd = fan;
        toggle(lastCmd);
        understoodCmd();
        break;     


      case sonu4: 
      case shaurya4:
      case dadi4:
        lastCmd = small;
        toggle(lastCmd);
        understoodCmd();
        break; 
         
      case sonu:
        /** turn on LED */
        
          myVR.clear();
          record[0] = sonu;
          record[1] = shaurya;
          record[2] = dadi;
          record[3] = sonu1;
          record[4] = sonu2;
          record[5] = sonu3;
          record[6] = sonu4;
       
          if(myVR.load(record, 7) >= 0){
            printRecord(record, 7);
            Serial.println(F("loaded."));
          }
        if (sonuLoaded){//User already loaded he wants to set timer
          setTimer();  
        }else{//new user loaded first time so clear previous timers if any set by other users 
          resetTimer(); 
        }
        shauryaLoaded = false;
        sonuLoaded = true;
        dadiLoaded = false;
        understoodCmd();
        break;

        case shaurya:
          myVR.clear();
          record[0] = sonu;
          record[1] = shaurya;
          record[2] = dadi;
          record[3] = shaurya1;
          record[4] = shaurya2;
          record[5] = shaurya3;
          record[6] = shaurya4;
       
          if(myVR.load(record, 7) >= 0){
            printRecord(record, 7);
            Serial.println(F("loaded."));
          }
        if (shauryaLoaded){//User already loaded he wants to set timer
          setTimer();  
        }else{//new user loaded first time so clear previous timers if any set by other users 
          resetTimer(); 
        }
        shauryaLoaded = true;
        sonuLoaded = false;
        dadiLoaded = false;
        understoodCmd();
        break;
        case dadi:
          myVR.clear();
          record[0] = sonu;
          record[1] = shaurya;
          record[2] = dadi;
          record[3] = dadi1;
          record[4] = dadi2;
          record[5] = dadi3;
          record[6] = dadi4;
       
          if(myVR.load(record, 7) >= 0){
            printRecord(record, 7);
            Serial.println(F("loaded."));
          }
        if (dadiLoaded){//User already loaded he wants to set timer
          setTimer();  
        }else{//new user loaded first time so clear previous timers if any set by other users 
          resetTimer(); 
        }
        shauryaLoaded = false;
        sonuLoaded = false;
        dadiLoaded = true;
        understoodCmd();
        break;
      default:
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf     --> command length
           len     --> number of parameters
*/
void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

/**
  @brief   Print signature, if the character is invisible, 
           print hexible value instead.
  @param   buf  -->  VR module return value when voice is recognized.
             buf[0]  -->  Group mode(FF: None Group, 0x8n: User, 0x0n:System
             buf[1]  -->  number of record which is recognized. 
             buf[2]  -->  Recognizer index(position) value of the recognized record.
             buf[3]  -->  Signature length
             buf[4]~buf[n] --> Signature
*/
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
//  Serial.println("\r\n");
  Serial.println();
}

void printRecord(uint8_t *buf, uint8_t len)
{
  Serial.print(F("Record: "));
  for(int i=0; i<len; i++){
    Serial.print(buf[i], DEC);
    Serial.print(", ");
  }
}
