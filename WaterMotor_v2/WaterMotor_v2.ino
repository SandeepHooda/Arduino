

#define led PD2
#define motorSignalInput PD3
#define relay PD4
#define gnd PD5
boolean tankFull = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(motorSignalInput, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(gnd, OUTPUT);
   
  digitalWrite(led, HIGH);
  digitalWrite(relay, HIGH);
  digitalWrite(gnd, LOW);
}


void loop() {
  if (tankFull){
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
    return;
  }
  if (digitalRead(motorSignalInput)){
    tankFull = true;
    digitalWrite(relay, LOW);
    digitalWrite(led, LOW);
  }
 
}



