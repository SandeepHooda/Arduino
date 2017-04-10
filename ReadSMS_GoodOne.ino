/*  
 *  GPRS/GSM Quadband Module (SIM900)
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  a
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           2.0
 *  Design:            David Gascón 
 *  Implementation:    Alejandro Gallego & Marcos Martinez
 */

int8_t answer;
int x;
int onModulePin= 2;
char aux_string[30];
char SMS[100];
String completeSMS ="";
char pin[] = "****";

void setup(){

    pinMode(onModulePin, OUTPUT);
    Serial.begin(115200);  

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
        while(Serial.available() == 0);
        // this loop reads the data of the SMS
        do{
            // if there are data in the UART input buffer, reads it and checks for the asnwer
            if(Serial.available() > 0){    
                SMS[x] = Serial.read();
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


void loop(){

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

    while( Serial.available() > 0) Serial.read();    // Clean the input buffer

    Serial.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        // if there are data in the UART input buffer, reads it and checks for the asnwer
        if(Serial.available() != 0){    
            response[x] = Serial.read();
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
    
