/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

String host     = "http://sandeephoodaiot.appspot.com"; // Your domain 
String pathToGetCommand          = "/GetData?collection=bedRoom_sonu.hooda@gmail.com"; //"/GetData?collection=test&datakey=bedRoom"; it should return json data like this : { "light" : "off" , "fan" : "on"} 
String pathToUpdate          = "/PutData?collection=microCtrl_bedRoom_sonu.hooda@gmail.com&datakey=microctrl&value=";


#define lightPin D5  
#define fanPin  D0  

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);
 pinMode(lightPin, OUTPUT); 
  pinMode(fanPin, OUTPUT); 
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("GREEN", "A0A0A0A0A0");

}

String getCommandOrReportStatus(boolean getCommand, String statusUpdate){
  String payload = "NULL";
  String response = "NULL";
  
  
  HTTPClient http;
  //USE_SERIAL.print("[HTTP] begin...\n");
  
  String resourcePath = host+pathToUpdate+statusUpdate;
  if (getCommand){
    resourcePath = host+pathToGetCommand;
  }
Serial.println("Resource path ="+resourcePath );
  http.begin(resourcePath); //HTTP
  //USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                USE_SERIAL.println(payload);
                if (getCommand){
                      int size = payload.length() ;
                      char json[size];
                      payload.toCharArray(json, size);
                      StaticJsonBuffer<200> jsonBuffer;
                      JsonObject& json_parsed = jsonBuffer.parseObject(json);
                      if (!json_parsed.success())
                      {
                        Serial.println("parseObject() failed");
                        return "parseObject failed";
                      }
        
                  // Make the decision to turn off or on the LED
                    if (strcmp(json_parsed["light"], "on") == 0) {
                      digitalWrite(lightPin, HIGH); 
                      response = "light:on";
                      Serial.println("LED ON");
                    } else if (strcmp(json_parsed["light"], "off") == 0) {
                      digitalWrite(lightPin, LOW);
                      response = "light:off";
                      Serial.println("led off");
                     }
            
                    if (strcmp(json_parsed["fan"], "on") == 0) {
                      digitalWrite(fanPin, HIGH); 
                      response += "_fan:on";
                      //Serial.println("FAN ON");
                     } else if (strcmp(json_parsed["fan"], "off") == 0) {
                      digitalWrite(fanPin, LOW);
                      response += "_fan:off";
                     // Serial.println("FAN off");
                     }
                }//End of if get command
      
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end();   
  return response; 
}

void loop() {  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        String commandexecutionStatus = getCommandOrReportStatus(true,"");//Get command to turn of or off
        getCommandOrReportStatus(false,commandexecutionStatus);//Update what micro controller did on that command
    }    
}
