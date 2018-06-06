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
String path          = "/MotionDetected";
String payload = "{  \"_id\": \"1\",   \"motionDetected\": \"true\" }";
int motionDetected =0;

void setup() {
  pinMode(D1, INPUT);
  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("GREEN", "A0A0A0A0A0");

}

String updateMdb(){
  

  HTTPClient http;
  String resourcePath = host+path;
  
  Serial.println("Resource path ="+resourcePath );
  http.begin(resourcePath); //HTTP
  //http.addHeader("Content-Type", "application/json");  
  int httpCode = http.GET(); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
              motionDetected = 0;
              USE_SERIAL.printf("[HTTP] POST... success, code: \n");
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end();   
 
}

void loop() {  
  if( digitalRead(D1)){
        motionDetected =1; 
    }
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {
       if(motionDetected == 1){
        updateMdb();
       }
    }    
}

