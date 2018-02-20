

#define led 2
#define motorSignalFull 9
#define motorSignalEmpty 8
#define relay 7
#define waterSourceAvailableIP 11
#define waterSourceAvailableOP 10

boolean tankFull = false;
boolean tankEmpty = false;
boolean checkingTankEmpty = false;

void setup() {
  Serial.begin(9600);
  pinMode(waterSourceAvailableIP, INPUT);
  pinMode(waterSourceAvailableOP, OUTPUT);
  pinMode(motorSignalFull, INPUT);
  pinMode(motorSignalEmpty, INPUT);
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

  if (!digitalRead(waterSourceAvailableIP)){
   turnOffMotor(100);//Water at source is not available
    return;
  }else {
    digitalWrite(led, HIGH);
  }
  if (digitalRead(motorSignalEmpty)){
    //At any point if we find high on motorSignalEmpty that means that empty signal is also being monitored
     checkingTankEmpty = true;
  }
  if (tankFull){
     turnOffMotor(500);
     if (checkingTankEmpty && !digitalRead(motorSignalEmpty)){
        turnOnMotor();
     }
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
    }
     
   }
 
}

void blinkLED(){
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(led, HIGH);
    delay(500);
    
}


