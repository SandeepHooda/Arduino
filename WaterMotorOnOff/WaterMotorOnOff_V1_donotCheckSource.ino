

#define led 2
#define motorSignalFull 9
#define waterAboveMinLevelIn_OHT 8
#define relay 7
#define waterSourceAvailableIP 11
#define waterSourceAvailableOP 10

boolean tankFull = false;
boolean tankEmpty = false;
boolean checkingTankEmpty = false;

void setup() {
  Serial.begin(9600);
  //pinMode(waterSourceAvailableIP, INPUT); //This pin is shorted to VCC to make it high all the time, To use it put the wire in source water tank
  pinMode(waterSourceAvailableOP, OUTPUT);
  pinMode(motorSignalFull, INPUT);
  pinMode(waterAboveMinLevelIn_OHT, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);

 digitalWrite(waterSourceAvailableOP, HIGH);
 
  turnOnMotor();
  delay(200);

}

void turnOnMotor(){
   digitalWrite(led, HIGH);
   digitalWrite(relay, HIGH);
   tankFull = false;
}

void turnOffMotor(int delayTime){
    digitalWrite(led, HIGH);
    delay(delayTime);
    digitalWrite(led, LOW);
    delay(delayTime);
    digitalWrite(relay, LOW);
}

void loop() {
delay(5000);//wait for 5 sconds
  /*if (!digitalRead(waterSourceAvailableIP)){
   turnOffMotor(100);//No water left to pump in storage (source of water is empty)
    return;
  }else {
    digitalWrite(led, HIGH);
  }*/
  if (digitalRead(waterAboveMinLevelIn_OHT)){
    //At any point if we find high on waterAboveMinLevelIn_OHT that means that empty signal is also being monitored
     checkingTankEmpty = true;
  }
  if (tankFull){
     turnOffMotor(500);
     //Motor was turned off because it OHT was full so check if it has still water left above the min level 
     if (checkingTankEmpty && !digitalRead(waterAboveMinLevelIn_OHT)){//checking if there still water left in Over Hear Tank (OHT) or not
        turnOnMotor();
     }
  }
  if (digitalRead(motorSignalFull)){
    digitalWrite(led, LOW);
    delay(10000);//wait for 10 sconds
    
    if (digitalRead(motorSignalFull)){
      tankFull = true;
    }else {
      digitalWrite(led, HIGH);
    }
     
   }
 
}

void blinkLED(){
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(500);
    
}


