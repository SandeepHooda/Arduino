

#define PIR1  PD3
#define PIR2  PD4  
 

#define relay  PD2 
int motionCount =0;
int switchOn = 0; 
int time =10;


  void setup()
{
  Serial.begin(9600);  

  pinMode(relay,OUTPUT );
  digitalWrite(relay, HIGH);//Off
  pinMode(PIR1, INPUT); 
  pinMode(PIR2, INPUT); 

Serial.println("Welcome");
}

void loop(){  
 
if ( digitalRead(PIR1) || digitalRead(PIR2) ){//motion detected
  Serial.print("Motion 1");
   Serial.println(digitalRead(PIR1));
    Serial.print("Motion 2");
     Serial.println(digitalRead(PIR2));
  if (switchOn < time){
    switchOn = time;//Since motion is deteted so sensor should be on for atleast 60 Seconds
  }
}

  if (switchOn > 0){
    digitalWrite(relay, LOW);//On
    switchOn--;
    delay(1000);
    
  }else {
    digitalWrite(relay, HIGH);//Off
    delay(200);
  }

   
  
}

