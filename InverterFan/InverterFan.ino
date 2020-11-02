
#define FAN 2
#define light_sensor  A5
#define led  13
int lightIntensity_ToStartFan = 500;
int lightIntensity_ToStopFan = 250;
int lightIntensity_Threshhold = 0;
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(FAN, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode (light_sensor,INPUT);
  Serial.begin(9600); 
  delay(200);
  lightIntensity_Threshhold = lightIntensity_ToStartFan;
}

boolean isNight(){
 int  lightIntensity =  analogRead(light_sensor);
 Serial.print("Light intensity ");
 Serial.println(lightIntensity); 
 if (lightIntensity < lightIntensity_Threshhold){
    //during night
    digitalWrite(led, HIGH);
    lightIntensity_Threshhold = lightIntensity_ToStartFan;
    return true;
  }else {
    //during day
    digitalWrite(led, LOW);
    lightIntensity_Threshhold = lightIntensity_ToStopFan;
    return false;
  }
}
// the loop function runs over and over again forever
void loop() {
  if (isNight()){
     digitalWrite(FAN, LOW);
  }else {
    digitalWrite(FAN, HIGH); 
  }
// delay(600000); 
  delay(1000);                       // wait for a second
}

