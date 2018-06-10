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

String host     = "http://192.168.225.21"; // Your domain 

boolean correctPosition = false;

void setup() {

  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("GREEN", "A0A0A0A0A0");

}

String rotateCamera(String where){
 
  String path          = "/decoder_control.cgi?command=1&user=sandeephooda&pwd=ForgetNot85";//Stop rotation
   if (where == "down"){
    path   = "/decoder_control.cgi?command=2&user=sandeephooda&pwd=ForgetNot85";
  }else if (where == "up"){
    path   = "/decoder_control.cgi?command=0&user=sandeephooda&pwd=ForgetNot85";
  }else if (where == "left"){
    path   = "/decoder_control.cgi?command=6&user=sandeephooda&pwd=ForgetNot85";
  }else if (where == "right"){
    path   = "/decoder_control.cgi?command=4&user=sandeephooda&pwd=ForgetNot85";
  }
  HTTPClient http;
  String resourcePath = host+path;
  
  Serial.println("Rotat = "+where );
  http.begin(resourcePath); //HTTP
  //http.addHeader("Content-Type", "application/json");  
  int httpCode = http.GET(); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
              
              USE_SERIAL.printf("[HTTP] GET... success, code: \n");
              if (where == "down"){
                correctPosition = true;
              }
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] rotation GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end();   
 
}

void clickPhoto(){

  HTTPClient http;
  String resourcePath = "http://remind-me-on-heroku.herokuapp.com/GetMotionStatus";
  
  Serial.println("Getting motion status  = "+resourcePath );
  http.begin(resourcePath); //HTTP
  //http.addHeader("Content-Type", "application/json");  
  int httpCode = http.GET(); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                if (payload.indexOf("\"motionDetected\" : \"true\"") > 0){
                  correctPosition = false;
                  rotateCamera("up");
                  delay(10000);
                  rotateCamera("down");
                  delay(10000);
                }
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] motion status GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end(); 
  
}
void positionAttainedStatusEmail(){
  String resourcePath = "http://remind-me-on-heroku.herokuapp.com/FoscamPosition?value=";
  if (correctPosition){
    resourcePath += "true";
  }else {
     resourcePath += "false";
  }
  HTTPClient http;
  Serial.println("positionAttainedStatusEmail = "+resourcePath );
  http.begin(resourcePath); //HTTP
  //http.addHeader("Content-Type", "application/json");  
  int httpCode = http.GET(); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
               
                USE_SERIAL.println("positionAttainedStatusEmail Sent");
               
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] positionAttainedStatusEmail status GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end();
}
void loop() {  
  if(millis() > 120000){//After two minutes
    if (!correctPosition){
      rotateCamera("down");
      delay(10000);
      rotateCamera("stop");
      positionAttainedStatusEmail();
      
    }else {//check if motion signal turned on my web user interface http://sandeephoodaiot.appspot.com/MotionDetected
      clickPhoto();
    }
    
    
  }
  
  
}

