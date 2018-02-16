

#define led 2
#define motorSignalFull 9
#define motorSignalEmpty 8
#define relay 7

boolean tankFull = false;
boolean tankEmpty = false;
boolean checkingTankEmpty = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(motorSignalFull, INPUT);
  pinMode(motorSignalEmpty, INPUT);
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
void turnOffMotor(){
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
    digitalWrite(relay, LOW);
}

void loop() {
  if (digitalRead(motorSignalEmpty)){
    //At any point if we find high on motorSignalEmpty that means that empty signal is also being monitored
     checkingTankEmpty = true;
  }
  if (tankFull){
     turnOffMotor();
     if (checkingTankEmpty && !digitalRead(motorSignalEmpty)){
        turnOnMotor();
     }
  }
  if (digitalRead(motorSignalFull)){
     tankFull = true;
   }
 
}



