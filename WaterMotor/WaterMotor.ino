

#define waterIndicator 10
#define relayT1 11
#define motorFullLed 12
boolean tankFull = false;

void setup() {
  Serial.begin(9600);
  pinMode(waterIndicator, INPUT);
  pinMode(relayT1, OUTPUT);
  pinMode(motorFullLed, OUTPUT);
  digitalWrite(relayT1, HIGH);
  digitalWrite(motorFullLed, LOW);
}


void loop() {
  if (tankFull){
    return;
  }
  if (digitalRead(waterIndicator)){
    tankFull = true;
    digitalWrite(relayT1, LOW);
    digitalWrite(motorFullLed, HIGH);
  }
 
}



