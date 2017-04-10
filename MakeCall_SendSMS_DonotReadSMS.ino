 #include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);//rx, tx
boolean printHelp = true;
boolean state, lastState;
int switchPin = 8;
int x;
char SMS[600];
void setup()
{
  pinMode(switchPin, INPUT_PULLUP);
  state = digitalRead(switchPin);
  lastState = state;
  
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.println("Welcome ");
 RecieveMessage();
  Serial.println("Done init ");
  
}
void loop(){
  
}

void loop2()
{
  lastState = state;
  state = digitalRead(switchPin);
  if ( state != lastState ) {
    if ( state ) {
       Serial.println("Making a call now ");
      Call();
      }else{
         Serial.println("Disconnecting now ");
        Disconnect();
      }
  }
  

  /*
  if (Serial.available()>0){
    if (printHelp){
      Serial.println("s - Send SMS ");
       Serial.println("c - make a call");
       printHelp = false;
    }
      
     switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'r':
      RecieveMessage();
      break;
     case 'c':

      Call();
      break;
  }
  }
  

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
   */
}


 void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  
  mySerial.println("AT+CMGS=\"9216411835\""); // Replace x with mobile number
  delay(1000);
  mySerial.println("I am SMS from ardrino");// The SMS text you want to send
  delay(500);
 mySerial.write(0x1A);  // sends ctrl+z end of message
   mySerial.write(0x0D);  // Carriage Return in Hex
mySerial.write(0x0A);  // Line feed in Hex
//The 0D0A pair of characters is the signal for the end of a line and beginning of another.
delay(5000);
  delay(500);
}


 void RecieveMessage()
{
  delay(1000);
  Serial.println("enter receive message");
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
   delay(1000);
   // mySerial.println("AT+CPMS=\"SM\",\"SM\",\"SM\""); 
   //delay(1000);
 
Serial.println("Read all message");
char response[1000];
memset(response, '\0', sizeof(response));    // Initialice the string
   int x=0;
   
   char* expected_answer = "+CMGL=\"ALL\"";
   Serial.println("entering while loop");
    mySerial.println("AT+CMGL=\"ALL\"");    //Read the third sms
    delay(1000);
   while(mySerial.available() >0){
    response[x] = mySerial.read();
            x++;
    if (strstr(response, expected_answer) != NULL) {
      
       break;
    }
   
   }
   Serial.println(response);
  int answer = 0;
 x=0;
 memset(response, '\0', sizeof(response));    // Initialice the string
  while (answer == 0){
     if(mySerial.available() >0){
      Serial.println(response); 
      response[x] = mySerial.read();
    x++;
    if(strstr(response, "OK") != NULL){
      answer = 1;
     }
     }
    
  }
  
  //SMS[x] = '\0';
        
        Serial.print(response);    
        Serial.println("--------End of program--------");  
   Serial.println("\nEnd of SMS");
 }

  void Call()
{
  mySerial.println("ATD9216411835;"); // AT Command to make a call
 
  delay(1000);
  if (mySerial.available()>0)
   Serial.println(mySerial.read());
 }

 void Disconnect(){
  mySerial.println("ATH"); // AT Command to make a call
  delay(1000);
  if (mySerial.available()>0)
   Serial.println(mySerial.read());
 }
 
