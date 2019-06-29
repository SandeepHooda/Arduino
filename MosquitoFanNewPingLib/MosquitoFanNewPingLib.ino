#include <NewPing.h>

#define PIR1_TRIGGER_PIN  PD5
#define PIR1_ECHO_PIN  PD6  
#define PIR2_TRIGGER_PIN  PD3
#define PIR2_ECHO_PIN  PD4 
#define relay  PD2 
int pirState = LOW;             // we start, assuming no motion detected
int switchOn = 0; 
long distance = 0;
#define MAX_DISTANCE 150
 
NewPing sonar(PIR1_TRIGGER_PIN, PIR1_ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(PIR2_TRIGGER_PIN, PIR2_ECHO_PIN, MAX_DISTANCE);
  void setup()
{
  Serial.begin(9600);  

  pinMode(relay,OUTPUT );
  digitalWrite(relay, HIGH);//Off
  Serial.println("Welcome  ");
}
void loop(){
 //if (switchOn <= 0){
delay(50);
distance = sonar.ping_cm();
if (distance > 10){
  Serial.println(distance);
  delay(50);
  distance = sonar.ping_cm();
  if (distance > 10){
    
  switchOn = 5;//Seconds
      /*delay(200);
      distance = sonar.ping_cm();
      if (distance > 10){
        Serial.print("Signal 3 ");
  Serial.println(distance);
          switchOn = 5;//Seconds
         
      }*/
    
      
     
  }
}/*else{ //check sonar2 
    distance = sonar2.ping_cm();
    if (distance != "0"){
      delay(50);
      distance = sonar2.ping_cm();
      if (distance != "0"){
          switchOn = 60;//Seconds
         
      }
    } 
}*/

//}
  
 
  
   

  if (switchOn > 0){
    digitalWrite(relay, LOW);//On
    switchOn--;
    delay(1000);
    
  }else {
    digitalWrite(relay, HIGH);//Off
    
  }

  
 
  
}

