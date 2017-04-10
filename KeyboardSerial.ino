/*
 Keyboard test

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
 #define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_LEFT_GUI   0x83
#define KEY_DELETE 0xD4
#define regularPwd 2
#define Infy 4
#define ctrl_ALt_DEL 3
#include <Keyboard.h>

void setup() {
  // open the serial port:
  Serial.begin(9600);
  // initialize control over the keyboard:
  Keyboard.begin();
  pinMode(regularPwd,INPUT);
}


 void loop(){
    delay(1000);
    
 }
void loop1() {
  if (digitalRead(ctrl_ALt_DEL)){
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_DELETE);
  delay(200);
  Keyboard.release(KEY_LEFT_CTRL);
  Keyboard.release(KEY_LEFT_ALT);
  Keyboard.release(KEY_DELETE);
  }
  if (digitalRead(regularPwd)){
    Keyboard.print("Sandeep@1234\n");
    delay(200);
  }
   if (digitalRead(Infy)){
   Keyboard.print("sandeep_hooda");
   Keyboard.print("\t");
   Keyboard.print("Nov@2016");
   Keyboard.print("\n");
    delay(200);
  }
   
    
}

