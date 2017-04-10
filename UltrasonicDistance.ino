#define trigPin 13  
#define echoPin 12
#define buzzer 3
int sound = 4000;
 int count = 0;
  int longDistance = 25;
  boolean carEntering = false;
  boolean showDisplay = false;
  int loopCount = 0;

void setup() {

 
  
  
 Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop() {
  Serial.println(" cm");
   long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

   if (distance<5){
    tone(buzzer, sound+100);
     Serial.print(distance);
         Serial.println(" cm");
   }else if (distance<10){
    tone(buzzer, sound+200);
     Serial.print(distance);
         Serial.println(" cm");
   }else if (distance<15){
     Serial.print(distance);
         Serial.println(" cm");
    tone(buzzer, sound+300);
   } else if (distance<20){
     Serial.print(distance);
         Serial.println(" cm");
    tone(buzzer, sound+400);
   }
    
 
   if (distance<longDistance){
        
        if (!carEntering){
          count++;
          Serial.print(distance);
         Serial.println(" cm");
         Serial.println(count);
        }
    
      carEntering = true;
  }else{
     noTone(buzzer);
    carEntering = false;
   }

   
  delay(10);
}
