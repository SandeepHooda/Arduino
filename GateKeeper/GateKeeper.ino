#include <SoftwareSerial.h>
#include <EEPROM.h>
SoftwareSerial mySerial(PD2, PD3);//rx, tx
SoftwareSerial bluetooth(PD6,PD5 ); // RX, TX  

//2 rx of arduinto -> tx of SIM module
//3 tx of arduinto -> tx of SIM  module
char epromPhoneNo[10];
String epromPhoneNoStr = "";
char callerID_phone_number[13];
boolean incommingIDFound = false;
String phoneNo;
  void setup()
{
  pinMode(PD4, INPUT);
  /*pinMode(PD5, OUTPUT);
  pinMode(PD6, OUTPUT);
  digitalWrite(PD6, LOW);*/
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
   bluetooth.begin(9600);
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.print("Welcome By GateKeeper. I will use phone no "); 
 // sendATcommand("AT+CMGD=1,1", "AT+CMGD=1,1", 1000); //Delete all SMS
 // Serial.println("All SMS deleted"); 
  for (int i=0;i<10;i++){
     epromPhoneNo[i] =  EEPROM.read(i);
  }
   String str(epromPhoneNo);
    epromPhoneNoStr = str;
 Serial.println(epromPhoneNoStr);
  //waitForIncommingCall();
  //readMasterPhoneNoFromSMS();
  
}

void readMasterPhoneNoFromSMS(){
 

  unsigned long waitForCallerID = 120000;
  while (!incommingIDFound && ( millis() < waitForCallerID )){
     readLatestSMS();
  }

   if (incommingIDFound){
    Serial.print("Caller ID  ");
   
   
   }
}
void waitForIncommingCall(){
  sendATcommand("AT+CLIP=1", "AT+CLIP=1", 1000);

  unsigned long waitForCallerID = 120000;
  while (!incommingIDFound && ( millis() < waitForCallerID )){
     getCallerID();
  }

   if (incommingIDFound){
    Serial.print("Caller ID  ");
   
    String phoneIDString(callerID_phone_number);
    phoneIDString = phoneIDString.substring(0,13);
    String str(epromPhoneNo);
    epromPhoneNoStr = str;
    epromPhoneNoStr = epromPhoneNoStr.substring(0,13);
    Serial.println(phoneIDString); 
      if ( !phoneIDString.equalsIgnoreCase(epromPhoneNoStr) ){
       for (int i=0;i<13;i++){
         EEPROM.write(i, callerID_phone_number[i]);
       }
       /*for (int i =0;i< 5;i++){
           digitalWrite(PD5, HIGH);
          delay(500);
          digitalWrite(PD5, LOW);
          delay(500);
       }*/
       epromPhoneNoStr = phoneIDString;
       
      }else {
         Serial.println("No need to store in eprom ");
      }
      
   }
 
}
void readLatestSMS() {
  int8_t answer;
  char SMS[100];
  int x;
String completeSMS ="";
 answer = sendATcommand("AT+CMGR=1", "AT+CMGR=1", 5000);    // reads the first SMS
    if (answer == 1)
    {
       Serial.println("Reading SMS now"); 
        answer = 0;
        while(mySerial.available() == 0);
        // this loop reads the data of the SMS
        Serial.println("Reading bytes");
        do{
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            if(mySerial.available() > 0){
                 
                SMS[x] = mySerial.read();
                x++;
                if (x>=sizeof(SMS)){
                  completeSMS += SMS;
                  memset(SMS, '\0', sizeof(SMS));
                  x=0;
                }
                // check if the desired answer (OK) is in the response of the module
                if (strstr(SMS, "OK") != NULL)    
                {
                    answer = 1;
                    completeSMS += SMS;
                }
            }
            delay(100); 
        }while(answer == 0);    // Waits for the asnwer with time out
        
        //SMS[x] = '\0';
        
        Serial.print(completeSMS);  
           
        Serial.println("--------End of program--------");  
        incommingIDFound = true;
        
    }
    else
    {
        Serial.print("error ");
        Serial.println(answer, DEC);
    }

  
}

void getCallerID() {
int8_t answer;

char received[15];

  //program is allways waiting for a +CLIP to confirm a call was received
  //it will receive a +CLIP command for every ring the calling phone does
  while (answer = sendATcommand("ID", "+CLIP:", 2000)) {
    //answer is 1 if sendATcommand detects +CLIP
    if (answer == 1)
    {
      Serial.println("Incoming call - store number");

      for (int i = 0; i < 15; i++) {
        
        //read the incoming byte:mySerial.available()
        while (mySerial.available() == 0)
        {
          delay (50);
        }
        //stores phone number 
        received[i] = mySerial.read();
         Serial.print( received[i]);
      }
      Serial.flush();
      byte j = 0;
      //phone number comes after quotes (") so discard all bytes until find'em
      while (received[j] != '"') j++;
      j++;
      for (byte i = 0; i < 13; i++) {
        callerID_phone_number[i] = received[i + j];
      }
       incommingIDFound = true;
        
    }

   
  }
  
}
void loop(){
  //Check if user wants to upadte phione no in eprom via bluetooth
   if (bluetooth.available()) {
    String  c  = bluetooth.readStringUntil('\n');
    if(c.indexOf("UPDATE") >=0){
       epromPhoneNoStr = c.substring(c.indexOf("UPDATE")+6);
      epromPhoneNoStr.trim();
      if (epromPhoneNoStr.length() > 10){
         epromPhoneNoStr = epromPhoneNoStr.substring(0,10);
      }
     
      char epromPhoneNoChar[11];
      epromPhoneNoStr.toCharArray(epromPhoneNoChar, 11) ;
      for (int i=0;i<10;i++){
         EEPROM.write(i,epromPhoneNoChar[i]);
         bluetooth.print(epromPhoneNoChar[i]);
       }
       bluetooth.println("Now I will use "+epromPhoneNoStr+" to make a call" );
    }else {
       bluetooth.println("I will use "+epromPhoneNoStr+" to make a call" );
    }
   }else {
     //bluetooth.println("I will use "+epromPhoneNoStr+" to make a call" );
   }

   
  if (digitalRead(PD4)){
    Serial.println("Making a call ");
    //Send Hign on PD1 of ESP8266
    Call();
    
    //digitalWrite(PD5, HIGH);
    //delay(2000);
    //digitalWrite(PD5, LOW);
    
    
  }
}
 
 void Call()
{
  
   
  /*int8_t answer=  sendATcommand("AT+CPBR=1", "+CPBR: 1,\"", 2000);  // Read no from SIM memory
 
  if (answer == 1){
    readNumber();
    
  }*/
  Serial.print("Phone No: ");
  if (epromPhoneNoStr.length() >=13){
     phoneNo = epromPhoneNoStr.substring(3);
  }else if (epromPhoneNoStr.length() ==10 ){
    phoneNo = epromPhoneNoStr;
  }
 
   Serial.println(phoneNo);
  if (phoneNo.length() == 10){
    String callCmd = "ATD"+phoneNo+";";
    mySerial.println(callCmd); // AT Command to make a call
    Serial.print(" Call cammand is ");
     Serial.println(callCmd);
  }else {
   
    mySerial.println("ATD9216411835;"); // AT Command to make a call
  }
  
 
  delay(1000);
  if (mySerial.available()>0)
   Serial.print("Response from serial");
   Serial.println(mySerial.read());
 }

 void readNumber(){
  char SMS_INPUT[10];
  int x;
  String completeSMS ="";
         int8_t answer = 0;
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
                if (strstr(SMS_INPUT, "\"") != NULL)    
                {
                    answer = 1;
                    completeSMS += SMS_INPUT;
                }
            }
        }while(answer == 0);    // Waits for the asnwer with time out
        
        //SMS[x] = '\0';
        completeSMS = completeSMS.substring(0,10);
       phoneNo=  completeSMS;
        
    
 }

 int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[1000];
    unsigned long previous;
 
    memset(response, '\0', sizeof(response));    // Initialice the string

    delay(100);

    while( mySerial.available() > 0){
      mySerial.read();    // Clean the input buffer
    }

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
                
              //Serial.println(response);
            }else {
             /*  Serial.print("Expected : ");
             Serial.println(expected_answer);
             Serial.print("AT command Answer : ");
              Serial.println(response);*/
             
              
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
