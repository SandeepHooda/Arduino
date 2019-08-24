#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;
const char* ssid = "GOGREEN"; //"GREEN_2.4" //"GOGREEN"
const char* password = "F1F1F1F1F1"; //"A0A0A0A0A0" //"F1F1F1F1F1"
boolean ipUpdateRequired = true;
String  myExternalIPaddress = "";
String updateIpAddress = "http://sanhoo-home-security.appspot.com/MyExternalIP?ip=";
const char* getMyIPUrl = "http://api.ipify.org/?format=json";
String updateHealthUrl = "http://sanhoo-home-security.appspot.com/IamAlive?id=";
long ipUpdateTime = 0;
long healthUpdateTime = 0;
boolean lastUpdateSentWithAlarmMode = false;
#define FF_Gallary D5
//#define FF_Stairs D6
int httpCode_D1 = 0;
//int httpCode_D2 = 0;

boolean onAlarmMode = false;

/**
 * Update IP to site
 */
void updateIP(){
   
    HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(updateIpAddress+myExternalIPaddress);  //Specify request destination
    int httpCode = http.GET();      
                                                                 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);  
      ipUpdateTime = millis();
      ipUpdateRequired = false;
    }
     
    http.end();   //Close connection   
    delay(1000);  
}

/**
 * Get my external IP 
 */
void getIP(){
        
    HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(getMyIPUrl);  //Specify request destination
    int httpCode = http.GET();      
                                                                 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      JsonObject& payloadObj = jsonBuffer.parseObject(payload);
      const char* ipResponse = payloadObj["ip"];
      myExternalIPaddress  =  reinterpret_cast<const char*>(ipResponse);
      Serial.println(myExternalIPaddress); 
      Serial.println("");
      Serial.print("MY External IP");
      Serial.println(myExternalIPaddress);  
      updateIP();
    }
     
    http.end();   //Close connection
    delay(1000); 
}



void setup () {
 
    Serial.begin(115200);
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
      
    WiFi.begin(ssid, password);
    Serial.println("");
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
     
    delay(500);
    Serial.print(".");
    ipUpdateRequired = true;
    }
    Serial.println("");
    Serial.print("MY Local IP");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP

    pinMode(FF_Gallary, INPUT);    
    //pinMode(FF_Stairs, INPUT);    
}

int updateHealth(String url){
  if (!onAlarmMode && !lastUpdateSentWithAlarmMode){
        if ( (millis() - healthUpdateTime ) <5000) {//Reduce number of health updates if not in alarm mode. Health check monitor script from google drives every minute 
                                                    //( and sleeps 2 times in a minutes)
                                                    //Consider few updates might got miss due to environmental issues so don't send too less updates
          return -1;
        }
   }
   HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(url);  //Specify request destination
    int httpCode = http.GET();      
                                                                 
    if (httpCode > 0) { //Check the returning code
      //String payload = http.getString();   //Get the request response payload
      //Serial.println(payload);  
    }
     
    http.end();   //Close connection 
    delay(1000); 
    return  httpCode; 
}



void loop() {

     if ( (millis() - ipUpdateTime ) > 900000) { //Every 15 minute
          ipUpdateRequired = true;   
     }
     if (ipUpdateRequired){
          getIP();//Get and update
     }
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
        String alarm = "Y";
        onAlarmMode = false;
        if (!digitalRead(FF_Gallary) /*|| !digitalRead(FF_Stairs)*/){
           onAlarmMode = true;
        }
        if (digitalRead(FF_Gallary)){
          alarm = "N";
        }else {
          alarm = "Y";
        }
        Serial.print("Door 1 alarm : ");
        Serial.println(alarm);
       httpCode_D1 = updateHealth(updateHealthUrl+"7&alarmTriggered="+alarm);

      /*  if (digitalRead(FF_Stairs)){
          alarm = "N";
        }else {
          alarm = "Y";
       }
        Serial.print("Door 2 alarm : ");
        Serial.println(alarm);
        httpCode_D2 = updateHealth(updateHealthUrl+"2&alarmTriggered="+alarm);
*/
        if (onAlarmMode){//Last updates sent on alarm mode successfully
          lastUpdateSentWithAlarmMode = true;
        }else {
          lastUpdateSentWithAlarmMode = false;
        }

        if (httpCode_D1 > 0  /*&& httpCode_D2 > 0*/){
          healthUpdateTime = millis();
        }
        
        
    }else {
      ipUpdateRequired = true;
    }
     
    delay(100);    
 
}


