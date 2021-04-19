
/*
 Keyboard 


 For the Arduino Leonardo, Micro or Due

 Reads a byte from the serial port, sends a keystroke back.
 The sent keystroke is one higher than what's received, e.g.
 if you send a, you get b, send A you get B, and so forth.

 The circuit:
 * none

 created 21 Oct 2011
 modified 27 Mar 2012
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/KeyboardSerial
 */

#include <Keyboard.h>
#include <Mouse.h>
#define KEY_PRNT_SCRN 0xCE
unsigned long time;
void setup() {
  // open the serial port:
 Serial.begin(9600);
  // initialize control over the keyboard:
  Keyboard.begin();
   //Mouse.begin();
 // pinMode(regularPwd,INPUT);
 pinMode(2, OUTPUT);
 pinMode(3, INPUT); 
    
}

void beep(){
 digitalWrite(2, HIGH);
 delayMicroseconds(400);
 digitalWrite(2, LOW);
}
 void loop(){

   
  if (digitalRead(3) == HIGH){
    beep();
    Keyboard.press(KEY_LEFT_GUI); 
    Keyboard.press(KEY_PRNT_SCRN); 
    Keyboard.releaseAll();
  }
   delay(50);
  
    
 }



