int light = 13;
int soundDetectedPin = 8; // Use Pin 8 as our Input
int soundDetectedVal = HIGH; // This is where we record our Sound Measurement
boolean bAlarm = false;

unsigned long lastSoundDetectTime; // Record the time that we measured a sound


int soundAlarmTime = 500; // Number of milli seconds to keep the sound alarm high
boolean triggerRelay = false;
boolean lightStatus = true;
int clapcount = 0;
unsigned long timeStart;
unsigned long timeEnd;
int noOfClapsCommands = 3;//No of claps required to consider it a command
int secondsThreahhold = 15;
int delayBeforeNextClap = 2000;

void setup ()
{
  Serial.begin(9600);  
  pinMode (soundDetectedPin, INPUT) ; // input from the Sound Detection Module
  pinMode(light, OUTPUT);
  Serial.println("Welcome");
}
void loop ()
{
  soundDetectedVal = digitalRead (soundDetectedPin) ; // read the sound 
  triggerRelay = false;
  if (soundDetectedVal == HIGH){
    
    clapcount = 1;
     Serial.println(clapcount);
     delay(delayBeforeNextClap);// Wait for some time so that a clap is not counted multiple times
     timeStart =  millis();
    while( (clapcount < noOfClapsCommands ) && ( (millis() - timeStart) < 1000*secondsThreahhold) ){//if clap count is 3 or 15 seonds is elapsed which ever is less
      
      soundDetectedVal = digitalRead (soundDetectedPin) ; //read the sound 
      if (soundDetectedVal == HIGH){
        
       clapcount++;
       Serial.println(clapcount);
       delay(delayBeforeNextClap);// Wait for some time so that a clap is not counted multiple times, also to reduce environment noice
      }
    }
   
    if (clapcount >= noOfClapsCommands){
      Serial.println(" Will execute this order and wait for new  ");
        triggerRelay = true;
      }else{
        Serial.println(" Nothing to execute  ");
        triggerRelay = false;
      }
    }
   

  if (triggerRelay){
     
    if (!lightStatus){
      digitalWrite(light, LOW);// in My Relay low means on
      lightStatus = true;
      
    }else{
      digitalWrite(light, HIGH);// in My Relay low means on
      lightStatus = false;
    }
    
  }
  
  
}
