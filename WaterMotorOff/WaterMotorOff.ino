

#define led 2
#define motorSignalFull 9

#define relay 7

boolean tankFull = false;


void setup() {
  Serial.begin(9600);
  
  pinMode(motorSignalFull, INPUT);
 
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);

 
  turnOnMotor();
  delay(200);

}

void turnOnMotor(){
   digitalWrite(led, HIGH);
   digitalWrite(relay, HIGH);
   tankFull = false;
}
void blinkLED(){
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(500);
    
}

void loop() {
 
  if (tankFull){
     blinkLED();
     return;
    
  }
  if (digitalRead(motorSignalFull)){
    for (int i=0;i<10;i++){
      if (digitalRead(motorSignalFull)){
         blinkLED();// 1 sec delay
      }else {
        break;
      }
      
    }
    if (digitalRead(motorSignalFull)){
      tankFull = true;
     digitalWrite(relay, LOW);
    }
     
   }
 
}



