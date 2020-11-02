#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

StaticJsonBuffer<200> jsonBuffer;
const char* ssid = "GREEN_2.4";
const char* password = "A0A0A0A0A0";
boolean ipUpdateRequired = true;
String  myExternalIPaddress = "";
String updateIpAddress = "http://idonotremember-app.appspot.com/MyExternalIP?floor=FF&ip=";
String getMyIPUrl = "http://api.ipify.org/?format=json";
String updateHealthUrl = "http://idonotremember-app.appspot.com/IamAlive?id=";
long ipUpdateTime = 0;
long ipUpdateThreahHoldTime = 900000;
long healthUpdateTime = 0;
boolean lastUpdateSentWithAlarmMode = false;
#define FF_Gallary D5
#define FF_Stairs D6

int httpCode_D1 = 0;
int httpCode_D2 = 0;
String ipUpDateTrace = "";

boolean onAlarmMode = false;

void(* resetFunc) (void) = 0;//declare reset function at address 0


String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}


/**
 * Update IP to site
 */
void updateIP(String ngipIP){
    
    ipUpDateTrace = ipUpDateTrace + " : Calling update IP "+ngipIP+" received ";
    HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(updateIpAddress+ngipIP);  //Specify request destination
    int httpCode = http.GET();      
    ipUpDateTrace = ipUpDateTrace + " : Update IP Response "+httpCode;                                                             
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      //Serial.println(payload);  
      ipUpDateTrace = ipUpDateTrace + " : payload "+payload;
      ipUpdateTime = millis();
      ipUpdateRequired = false;
      http.end();   //Close connection
    }else {
      resetFunc(); //call reset 
    }
    //http.end();   //Close connection 
       
    delay(1000);  
}

/**
 * Get my external IP 
 */
void getIP(){
    
    HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(getMyIPUrl+"&ipUpdateTime="+ipUpdateTime);  //Specify request destination
    int httpCode = http.GET();      
                                                              
    if (httpCode == 200) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      JsonObject& payloadObj = jsonBuffer.parseObject(payload);
      const char* ipResponse = payloadObj["ip"];
      myExternalIPaddress  =  reinterpret_cast<const char*>(ipResponse);
      ipUpDateTrace = ": ngip IP = "+myExternalIPaddress;
     
      updateIP(myExternalIPaddress);
    }
     
    http.end();   //Close connection
    delay(1000); 
}



void setup () {

    delay(200);
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
    pinMode(FF_Stairs, INPUT);    
}

int updateHealth(String url){
 
  Serial.print("Update health Url ");
  Serial.println(url);
  if (!onAlarmMode && !lastUpdateSentWithAlarmMode){
        if ( (millis() - healthUpdateTime ) <5000) {//Reduce number of health updates if not in alarm mode. Health check monitor script from google drives every minute 
                                                    //( and sleeps 2 times in a minutes)
                                                    //Consider few updates might got miss due to environmental issues so don't send too less updates
          return -1;
        }
   }
   Serial.println("Will send the heatbeat ");
   HTTPClient http;  //Declare an object of class HTTPClient 
    http.begin(url);  //Specify request destination
    int httpCode = http.GET();      
    Serial.print("Heart beat http code");   
    Serial.println(httpCode);                                                            
    if (httpCode > 0) { //Check the returning code
      //String payload = http.getString();   //Get the request response payload
      //Serial.println(payload);  
    }
     
    http.end();   //Close connection 
    delay(1000); 
    return  httpCode; 
}



void loop() {
     long timeSinceIpUpdate = millis() - ipUpdateTime;
     //if this below don't work then use the count the nth iteration of loop 
     if ( timeSinceIpUpdate > ipUpdateThreahHoldTime) { //Every 15 minute 
          ipUpdateRequired = true;   
     }
     if (ipUpdateRequired){
          
          getIP();//Get and update
     }
    if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
        String alarm = "Y";
        onAlarmMode = false;
        if (!digitalRead(FF_Gallary) || !digitalRead(FF_Stairs)){
           onAlarmMode = true;
        }
        if (digitalRead(FF_Gallary)){
          alarm = "N";
        }else {
          alarm = "Y";
        }
        Serial.print("Door 1 alarm : ");
        Serial.println(alarm);
         
       httpCode_D1 = updateHealth(updateHealthUrl+"1&alarmTriggered="+alarm+"&ipUpDateTrace="+urlencode(ipUpDateTrace));

        if (digitalRead(FF_Stairs)){
          alarm = "N";
        }else {
          alarm = "Y";
       }
        Serial.print("Door 2 alarm : ");
        Serial.println(alarm);
        httpCode_D2 = updateHealth(updateHealthUrl+"2&alarmTriggered="+alarm+"&timeSinceIpUpdate="+timeSinceIpUpdate+"&ipUpdateRequired="+ipUpdateRequired);

        if (onAlarmMode){//Last updates sent on alarm mode successfully
          lastUpdateSentWithAlarmMode = true;
        }else {
          lastUpdateSentWithAlarmMode = false;
        }

        if (httpCode_D1 > 0  && httpCode_D2 > 0){
          healthUpdateTime = millis();
        }
        
        
    }else {
      ipUpdateRequired = true;
    }
     
    delay(100);    
 
}


