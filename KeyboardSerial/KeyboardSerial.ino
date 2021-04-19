
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
unsigned long time;
void setup() {
  // open the serial port:
 Serial.begin(9600);
  // initialize control over the keyboard:
  Keyboard.begin();
   //Mouse.begin();
 // pinMode(regularPwd,INPUT);
    
}


 void loop(){
  
  
    Mouse.move(110, 110, 0);
    delay(500);
    Mouse.move(-110, -110, 0);
    delay(500);
   
  
   /*Keyboard.print("S");

   delay(200);
    Keyboard.press(KEY_BACKSPACE);
    delay(200);
   Keyboard.release(KEY_BACKSPACE);*/
    
    
 }



