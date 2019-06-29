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

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

String host     = "http://remind-me-on-heroku.herokuapp.com"; // Your domain 
String pathToGetCommand          = "/AlexaWeMos?collection=AlexaSwitch_sonu.hooda@gmail.com&Switch=parking_light,ramp_light";  



#define parking_light D6  
#define ramp_light  D5  

void setup() {

    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);
 pinMode(parking_light, OUTPUT); 
  pinMode(ramp_light, OUTPUT); 
  digitalWrite(parking_light, HIGH);
  digitalWrite(ramp_light, HIGH);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    WiFiManager wifiManager;

    wifiManager.autoConnect("ESP8266");

}

void getSwitchState(){
  String payload = "NULL";
 
  
  
  HTTPClient http;
  //USE_SERIAL.print("[HTTP] begin...\n");
  
  String resourcePath = host+pathToGetCommand;
  
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
                
                      int size = payload.length() ;
                      char json[size];
                      payload.toCharArray(json, size);
                      StaticJsonBuffer<200> jsonBuffer;
                      JsonObject& json_parsed = jsonBuffer.parseObject(json);
                      if (!json_parsed.success())
                      {
                        Serial.println("parseObject() failed");
                        return;
                      }
        
                  // Make the decision to turn off or on the LED
                    if (strcmp(json_parsed["parking_light"], "on") == 0) {
                        digitalWrite(parking_light, LOW); 
                        Serial.println("parking_light ON");
                    } else if (strcmp(json_parsed["parking_light"], "off") == 0) {
                      digitalWrite(parking_light, HIGH);
                      Serial.println("parking_light off");
                     }
            
                    if (strcmp(json_parsed["ramp_light"], "on") == 0) {
                        digitalWrite(ramp_light, LOW); 
                        Serial.println("ramp_light ON");
                    } else if (strcmp(json_parsed["ramp_light"], "off") == 0) {
                      digitalWrite(ramp_light, HIGH);
                      Serial.println("ramp_light off");
                     }
              
      
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end();   
 
}

void loop() {  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
        getSwitchState();
        delay(1000);
      
    }    
}
