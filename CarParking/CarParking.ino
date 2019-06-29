
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x3F // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

#define trigPinSide 12  
#define echoPinSide 13
#define sideCool 11
#define sideOK 10
#define sideAlert 9 
#define trigPinFront 7  
#define echoPinFront 8
#define frontCool 6
#define frontOK 5
#define frontAlert 4
int distanceFront = 30;
int distanceSide = 12;
long distanceOfCarSide = 0;
long distanceOfCarFront = 0;
long duration = 0;
int tolerance = 15;
void setup() {

 
  Serial.begin (9600);
  pinMode(trigPinSide, OUTPUT);
  pinMode(echoPinSide, INPUT);
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);

  lcd.begin (16,2); //  <<----- My LCD was 16x2
  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); // go home
  Serial.println(" Welcome ");
  printWelcomeMessage();
  printWelcomeMessage();
  
}
void printWelcomeMessage(){
    digitalWrite(frontCool, LOW);
     digitalWrite(frontOK, LOW);
     digitalWrite(frontAlert, LOW);
     digitalWrite(sideCool, LOW);
     digitalWrite(sideOK, LOW);
     digitalWrite(sideAlert, LOW);

  
  digitalWrite(frontCool, HIGH);
  digitalWrite(sideCool, HIGH);
  delay(500);
  digitalWrite(frontOK, HIGH);
  digitalWrite(sideOK, HIGH);
  delay(500);
  digitalWrite(frontAlert, HIGH);
  digitalWrite(sideAlert, HIGH);
  delay(500);
   digitalWrite(frontAlert, LOW);
  digitalWrite(sideAlert, LOW);
  delay(500);
  digitalWrite(frontOK, LOW);
  digitalWrite(sideOK, LOW);
  delay(500);
  digitalWrite(frontCool, LOW);
  digitalWrite(sideCool, LOW);
  delay(500);
}
void loop() {

  //***************  Side distance
   /*
    digitalWrite(sideCool, LOW);
    digitalWrite(sideOK, LOW);
    digitalWrite(sideAlert, LOW);
    */
  digitalWrite(trigPinSide, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPinSide, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPinSide, LOW);
  duration = pulseIn(echoPinSide, HIGH);
  distanceOfCarSide = (duration/2) / 29.1;
 Serial.print("Side distance =");
 Serial.println(distanceOfCarSide);
  if ( distanceOfCarSide > (tolerance +distanceSide)){
    //Serial.println(" cool from side ");
    digitalWrite(sideCool, HIGH);
    digitalWrite(sideOK, LOW);
    digitalWrite(sideAlert, LOW);
  }else if ( ( distanceOfCarSide <= (tolerance +distanceSide)) && ( distanceOfCarSide > distanceSide)){
     //Serial.println(" ok from side");
    digitalWrite(sideOK, HIGH);
    digitalWrite(sideCool, LOW);
    digitalWrite(sideAlert, LOW);
  }else if ( (distanceOfCarSide <= distanceSide) && (distanceOfCarSide  > 0)){
     //Serial.println(" alert from side");
    digitalWrite(sideAlert, HIGH);
    digitalWrite(sideCool, LOW);
    digitalWrite(sideOK, LOW);
    
  }else if(distanceOfCarSide  == 0) { // show that device ultra sonic sensors not working
     digitalWrite(sideCool, HIGH);
     digitalWrite(sideOK, HIGH);
     digitalWrite(sideAlert, HIGH);
  }
  //***************  Front distance
  /*
  digitalWrite(frontCool, LOW);
    digitalWrite(frontOK, LOW);
    digitalWrite(frontAlert, LOW);
    */
  digitalWrite(trigPinFront, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPinFront, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPinFront, LOW);
  duration = pulseIn(echoPinFront, HIGH);
  distanceOfCarFront = (duration/2) / 29.1;
  Serial.print("Front Distance = ");
Serial.println(distanceOfCarFront);
  if ( distanceOfCarFront > (tolerance +distanceFront)){
    digitalWrite(frontCool, HIGH);
    digitalWrite(frontOK, LOW);
    digitalWrite(frontAlert, LOW);
  }else if ( ( distanceOfCarFront <= (tolerance +distanceFront)) && ( distanceOfCarFront > distanceFront)){
    digitalWrite(frontOK, HIGH);
    digitalWrite(frontCool, LOW);
    digitalWrite(frontAlert, LOW);
  }else if ((distanceOfCarFront <= distanceFront) && (distanceOfCarFront > 0)){
    digitalWrite(frontAlert, HIGH);
    digitalWrite(frontCool, LOW);
    digitalWrite(frontOK, LOW);
  }else if (distanceOfCarFront == 0){// show that device ultra sonic sensors not working
     digitalWrite(frontCool, HIGH);
     digitalWrite(frontOK, HIGH);
     digitalWrite(frontAlert, HIGH);
  }

    /*lcd.setCursor (0,0); 
    lcd.print("Front ");
    lcd.print(distanceOfCarFront);
    lcd.setCursor (0,1);        // go to start of 2nd line
    int a = 10;
    lcd.print("Side  ");
    lcd.print(distanceOfCarSide);*/
   if(distanceOfCarFront > 0 &&  distanceOfCarSide > 0){
    delay(500);
   }else{
    Serial.println("Recalculating ");
   }
  
}
