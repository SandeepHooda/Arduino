
#define motion_sensor  D0
#define light_sensor  A0
#define relay_control  D5
int tenMinutes = 10*60*1000;
int oneMinutes = 60*1000;
int halfMinute = 30*1000;
int threeMinutes = 3*60*1000;
int second = 1000;
int monitoringTimer = 0;
void setup(){
  pinMode (motion_sensor,INPUT);
  pinMode (relay_control, OUTPUT);
   if  ( isNight()   )   {
    digitalWrite (relay_control,HIGH);
   }else {
    digitalWrite (relay_control,LOW);
   }
  
  Serial.begin(9600); 
   delay(200);
  Serial.println("Welcome"); 
  delay(oneMinutes);
  Serial.println("let us start"); 
}

boolean isMotion(){
  int motionSensor_var = digitalRead(motion_sensor);
  //Serial.print("IS motion ");
 //Serial.println(motionSensor_var); 
  if (motionSensor_var == HIGH){
    return true;
  }else {
    return false;
  }
}
boolean isNight(){
 int  lightIntensity =  analogRead(light_sensor);
 //Serial.print("Light intensity ");
 //Serial.println(lightIntensity); 
 if (lightIntensity < 40){
    return true;
  }else {
    return false;
  }
}
void loop (){
  
  delay(100); //this delay is to let the sensor settle down before taking a reading
  //isNight();
  if (isMotion()){// motion yes
  //Serial.println("There is motion");   
  if  ( isNight()   )   {//Night yes
      digitalWrite (relay_control,HIGH);
      monitoringTimer = halfMinute;//Monitor timer 
      while(monitoringTimer >0 ){//runs for N minutes and sencr motion withinh this period
        // isNight();
        monitoringTimer = monitoringTimer -second;
        //Serial.print("Timer left ");
         //Serial.println(monitoringTimer);
        delay(second);
         if (isMotion()){// motion yes 
          monitoringTimer = halfMinute; 
         }
      }
   }else {//Night no
     digitalWrite (relay_control,LOW);
     }
  }else {// No motion 
     digitalWrite (relay_control,LOW);
     
  }
}

