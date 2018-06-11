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

String host     = "http://192.168.225.1/"; // Your domain 
String token = "";
String ip = "";
boolean successUpdate = false;
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
void updateWanInfo(String ip){
  HTTPClient http;
  String resourcePath = "http://remind-me-on-heroku.herokuapp.com/UpdateWanInfo?ip="+ip;
  
  Serial.println("Getting motion status  = "+resourcePath );
  http.begin(resourcePath); //HTTP
  

  int httpCode = http.GET(); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                successUpdate = true;
                
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] motion status GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end(); 
}
void getWanInfo(String token){

  HTTPClient http;
  String resourcePath = "http://192.168.225.1/cgi-bin/qcmap_web_cgi";
  
  Serial.println("Getting motion status  = "+resourcePath );
  http.begin(resourcePath); //HTTP
  http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");  
  http.addHeader("Accept", "text/plain, */*; q=0.01");  
  http.addHeader("X-Requested-With", "XMLHttpRequest");  

  int httpCode = http.POST("Page=GetWANInfo&mask=0&token="+token); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... getWanInfo: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                //"ipv4_addr":"10.83.174.199","ipv4_subnet"
                int start = payload.indexOf("\"ipv4_addr\":\"") + 13;
                int endindex = payload.indexOf("\",\"ipv4_subnet\"") ;
                ip = payload.substring(start,endindex);
                USE_SERIAL.println(ip);
                updateWanInfo(ip);
                
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] getWanInfo GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end(); 
  
}

void doLogin(){

  HTTPClient http;
  String resourcePath = "http://192.168.225.1/cgi-bin/qcmap_auth";
  
  Serial.println("Getting motion status  = "+resourcePath );
  http.begin(resourcePath); //HTTP
  http.addHeader("Content-Type", "application/x-www-form-urlencoded; charset=UTF-8");  
  http.addHeader("Accept", "text/plain, */*; q=0.01");  
  http.addHeader("X-Requested-With", "XMLHttpRequest");  

  int httpCode = http.POST("type=login&pwd=sandeep&timeout=600&user=sandeep"); 
    
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... doLogin: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.println(payload);
                int start = payload.indexOf("\"token\":\"") + 9;
                int endindex = payload.indexOf("\",\"level\"") ;
                token = payload.substring(start,endindex);
                USE_SERIAL.println(token);
                getWanInfo(token);
                
            }//http ok
        }else {
            USE_SERIAL.printf("[HTTP] doLogin GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        } 

        http.end(); 
  
}



void loop() { 
  successUpdate = false; 
  doLogin();
  if (successUpdate){
    delay(600000);//10 Minutes
  }else {
    delay(1000);//1 sec
  }
  
 
}

