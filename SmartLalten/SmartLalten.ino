#define motion_1  D0
#define night  A0
#define light_1  D5
void setup(){
  pinMode (motion_1,INPUT);

  pinMode (light_1, OUTPUT);
   digitalWrite (light_1,LOW);
  Serial.begin(9600); 
}

void loop (){
 
  delay(1000); //this delay is to let the sensor settle down before taking a reading
  int motion_sensor = digitalRead(motion_1);
  Serial.print("Ready night value: ");
  int nightSensor =  analogRead(night);
  Serial.println(nightSensor);

  boolean lightOn = false;
  if (motion_sensor == HIGH) {
    if (nightSensor < 50){
      lightOn = true;
    }else {
      Serial.println("Day time"); 
    }
  }

  if (lightOn){
    Serial.println("High ###################"); 
    digitalWrite(light_1,HIGH);
    delay(5000);
  }else{
     digitalWrite(light_1,LOW);
  }
    
       
   
  
}
