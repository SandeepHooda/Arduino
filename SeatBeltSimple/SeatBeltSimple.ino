#define driverSeatBeltSignalPin 12
#define pillionSeatBeltSignalPin  13
#define advisoryPin  8



void setup() {
  Serial.begin(9600);
  pinMode(driverSeatBeltSignalPin, INPUT);
  pinMode(pillionSeatBeltSignalPin, INPUT);
  pinMode(advisoryPin, OUTPUT);
  
  delay(1000);
 
  Serial.println("Welcome");
 
}

// the loop function runs over and over again forever
void loop() {

  delay(1000);
  if(   !digitalRead(driverSeatBeltSignalPin)  ||  !digitalRead(pillionSeatBeltSignalPin) ) // driver or  pillion not waering seat belt 
 
    {//Any seat bealt off , play advisory 
       playSeatBeltAdvisory();
    } 
     
  
}


void playSeatBeltAdvisory(){
  digitalWrite(advisoryPin, LOW);
  delay(10); 
  digitalWrite(advisoryPin, HIGH);
  delay(10 *1000);//10 Second
  digitalWrite(advisoryPin, LOW);
}

