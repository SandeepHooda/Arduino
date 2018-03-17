#include <SoftwareSerial.h>
SoftwareSerial mySerial(PD2, PD3);//rx, tx

//2 rx of arduinto -> tx of SIM module
//3 tx of arduinto -> tx of SIM  module
String phoneNo;
  void setup()
{
  pinMode(PD4, INPUT);
  pinMode(PD5, OUTPUT);
  pinMode(PD6, OUTPUT);
  digitalWrite(PD6, LOW);
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.println("Welcome By GateKeeper");
 
  
}
void loop(){
  if (digitalRead(PD4)){
    Serial.println("Making a call ");
    Call();
    digitalWrite(PD5, HIGH);
    delay(2000);
    digitalWrite(PD5, LOW);
    
    
  }
}
 
 void Call()
{
  
   
  int8_t answer=  sendATcommand("AT+CPBR=1", "+CPBR: 1,\"", 2000);  
 
  if (answer == 1){
    readNumber();
    
  }
  Serial.print("Phone No: ");
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
                
              //Serial.println(response);
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
