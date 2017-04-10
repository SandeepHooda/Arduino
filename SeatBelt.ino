int pillionSeatPersonDistance = 0;
boolean isPillionOnSeat = false;//Assumes that pillon is off seat when car started
int livePersonLeastMovementDistance = 3;//Live person will move atleat this much
int personMiniWidth = 10;//Person is atleast 10 cm wide
#define driverSeatBeltSignalPin 12
#define pillionSeatBeltSignalPin  13
int footMatDistance = 40;//Distance of foor mat from sensor
#define ultrasonicTriggerPin 6 
#define ultrasonicEchoPin 7 
#define advisoryPin  8

unsigned long lastTimeMotionDetected = 0;


void setup() {
  Serial.begin(9600);
  pinMode(driverSeatBeltSignalPin, INPUT);
  pinMode(pillionSeatBeltSignalPin, INPUT);
  pinMode(ultrasonicTriggerPin, OUTPUT);
  pinMode(ultrasonicEchoPin, INPUT);
  pinMode(advisoryPin, OUTPUT);
  delay(1000);
  pillionSeatPersonDistance = getDistance();
  Serial.println("Welcome");
 
}

// the loop function runs over and over again forever
void loop() {
  
 // if (digitalRead(driverSeatBeltSignalPin) == HIGH ){
    Serial.print("Driver === ");
    Serial.println(digitalRead(driverSeatBeltSignalPin));
    
  //}
//  if (digitalRead(pillionSeatBeltSignalPin) == HIGH){
    Serial.print("Pillion ===  ");
    Serial.println(digitalRead(pillionSeatBeltSignalPin));
 // }
  
  delay(1000);
  if( 
      !digitalRead(driverSeatBeltSignalPin)  || //Driver is not wearing seat belt
      (isPillionOnSeat && !digitalRead(pillionSeatBeltSignalPin) ) // pillion on seat but seat belt is off 
    )
    {//Any seat bealt off , play advisory 
       playSeatBeltAdvisory();
    } 
    if (!digitalRead(pillionSeatBeltSignalPin)){//Pillion is not wearing seat belt check if he is on seat
      
      isPillionSittingOnSeat();
    }
  
  
}

///footMatDistance
void isPillionSittingOnSeat(){
  delay(200);
  int pillionSeatDistanceNew = getDistance();
  Serial.println("---------");
Serial.println(pillionSeatDistanceNew);
Serial.println("#############");
//delay(1000);
  //3. Foor mat distance is less than threshhold
  if (pillionSeatDistanceNew < footMatDistance){
    isPillionOnSeat = true;
  }else {
    isPillionOnSeat = false;
  }
  //1. Live person detected
 /* if (
      ( pillionSeatPersonDistance - pillionSeatDistanceNew) > livePersonLeastMovementDistance || 
      ( pillionSeatDistanceNew  - pillionSeatPersonDistance ) > livePersonLeastMovementDistance
     ){//Assumes that a live person will move more that 3 cm
    isPillionOnSeat = true;
    lastTimeMotionDetected = millis();
  }else {
    if ( (millis() - lastTimeMotionDetected) > 60 * 1000){//If no motion for a minute 
      isPillionOnSeat = false;
    }
  }*/

/*
//2. Is person getting off seat
  if ( (pillionSeatPersonDistance - pillionSeatDistanceNew) > personMiniWidth){
    isPillionOnSeat = false;
    delay(1000 * 60);//Give person this much time to get off seat
  }
*/
  pillionSeatPersonDistance = pillionSeatDistanceNew;
}
//Returns the distance of object in front of sensor
long getDistance(){
  digitalWrite(ultrasonicTriggerPin, LOW); 
  delayMicroseconds(10); // Added this line
  digitalWrite(ultrasonicTriggerPin, HIGH);

  delayMicroseconds(10); 
  digitalWrite(ultrasonicTriggerPin, LOW);
long  duration = pulseIn(ultrasonicEchoPin, HIGH);
 long  distance = (duration/2) / 29.1;
 return distance;
}

void playSeatBeltAdvisory(){
  digitalWrite(advisoryPin, LOW);
  delay(10); 
  digitalWrite(advisoryPin, HIGH);
  delay(10 *1000);//10 Second
  digitalWrite(advisoryPin, LOW);
}

