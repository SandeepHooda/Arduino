 #include <SoftwareSerial.h>

SoftwareSerial mySerial(5, 6);//rx, tx 
//5 rx of arduinto -> tx of module
//6 tx of arduinto -> tx of module
boolean printHelp = true;
boolean state, lastState;
int switchPin = 8;
int x;
char SMS[600];

int8_t answer;
int onModulePin= 2;
char aux_string[30];
char SMS_INPUT[100];
String completeSMS ="";
char pin[] = "****";
void setup()
{
  pinMode(switchPin, INPUT_PULLUP);
  state = digitalRead(switchPin);
  lastState = state;
  
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(115200);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.println("Welcome ");

  Serial.println("Done init ");
  setupForReadSMS();
  //Call();
  
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



  void Call()
{
  mySerial.println("ATD9216411835;"); // AT Command to make a call
 
  delay(1000);
  if (mySerial.available()>0)
   Serial.println(mySerial.read());
   Serial.println("Done call ");
 }

 void Disconnect(){
  mySerial.println("ATH"); // AT Command to make a call
  delay(1000);
  if (mySerial.available()>0)
   Serial.println(mySerial.read());
 }


void setupForReadSMS(){

    
    pinMode(onModulePin, OUTPUT);
   

    Serial.println("Starting...");
    power_on();
    
    
    delay(3000);
    
    Serial.println("Setting SMS mode...");
    sendATcommand("AT+CMGF=1", "OK", 1000);    // sets the SMS mode to text
    sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 1000);    // selects the memory

    answer = sendATcommand("AT+CMGL=\"ALL\"", "+CMGL=\"ALL\"", 5000);    // reads the first SMS
    if (answer == 1)
    {
        answer = 0;
        while(mySerial.available() == 0);
        // this loop reads the data of the SMS
        do{
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            if(mySerial.available() > 0){    
                SMS_INPUT[x] = mySerial.read();
                x++;
                if (x>=sizeof(SMS_INPUT)){
                  completeSMS += SMS_INPUT;
                  memset(SMS_INPUT, '\0', sizeof(SMS_INPUT));
                  x=0;
                }
                // check if the desired answer (OK) is in the response of the module
                if (strstr(SMS_INPUT, "OK") != NULL)    
                {
                    answer = 1;
                    completeSMS += SMS_INPUT;
                }
            }
        }while(answer == 0);    // Waits for the asnwer with time out
        
        //SMS[x] = '\0';
        
        Serial.print(completeSMS);  
           
        Serial.println("--------End of program--------");  
        
    }
    else
    {
        Serial.print("error ");
        Serial.println(answer, DEC);
    }


}
void power_on(){

    uint8_t answer=0;
    
    // checks if the module is started
    answer = sendATcommand("AT", "OK", 2000);
    if (answer == 0)
    {
        // power on pulse
        digitalWrite(onModulePin,HIGH);
        delay(3000);
        digitalWrite(onModulePin,LOW);
    
        // waits for an answer from the module
        while(answer == 0){     // Send AT every two seconds and wait for the answer
            answer = sendATcommand("AT", "OK", 2000);    
        }
    }
    
}

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[1000];
    unsigned long previous;

    memset(response, '\0', sizeof(response));    // Initialice the string

    delay(100);

    while( mySerial.available() > 0) mySerial.read();    // Clean the input buffer

    mySerial.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(mySerial.available() != 0){    
            response[x] = mySerial.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer) != NULL)    
            {
                answer = 1;
            }else {
              //Serial.print("Answer : ");
              //Serial.println(response);
              // Serial.print("Expected : ");
             // Serial.println(expected_answer);
              
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));
   /* Serial.println("-------------------");    
            Serial.print("Answer : ");
              Serial.println(response);
               Serial.print("Expected : ");
              Serial.println(expected_answer);
              Serial.println("-------------------");    */
    return answer;
}
