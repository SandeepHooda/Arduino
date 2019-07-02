//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid = "Sandeep";
const char* password = "1234567890";


String host     = "http://sandeephoodaiot.appspot.com/VoltageLogger?voltage=";//"http://voltage-logger.herokuapp.com/VoltageLogger?voltage="; // Your domain 
String host_post     = "http://sandeephoodaiot.appspot.com/VoltageLogger";
String current_Volts;
boolean voltagedata = false;
void setup () {
 
Serial.begin(9600);
WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
//WiFi.config(staticIP, subnet, gateway, dns);
WiFi.persistent(false);
WiFi.disconnect(true);
WiFi.begin(ssid, password);
//WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only
while (WiFi.status() != WL_CONNECTED) {
 
delay(1000);
Serial.println("Connecting..");
 
}

}

void sendVoltageToDB(){

  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
  
 
 String url = host+current_Volts;
HTTPClient http;  //Declare an object of class HTTPClient



http.begin(url);  //Specify request destination
int httpCode = http.GET();                                                                  //Send the request
 
if (httpCode > 0) { //Check the returning code
 
String payload = http.getString();   //Get the request response payload
Serial.println(payload);                     //Print the response payload
 
}else{
  Serial.print("GET requet fail " );  
  Serial.println(httpCode ); 
}
 
http.end();   //Close connection
 
}
  
}
void loop() {

 if (Serial.available())  {
    char c = Serial.read();  //gets one byte from serial buffer
    if ( c == '#'){
      voltagedata = true;
    }

    if (voltagedata){
      if (c == ',') {
      if (current_Volts.length() >1) {
        voltagedata = false;
        Serial.print(current_Volts); //prints string to serial port out
        sendVoltageToDB();
        //do stuff with the captured readString 
        current_Volts=""; //clears variable for new input
        
           
      }
    }  
    else {     
      if ( c != '#'){
        current_Volts += c; //makes the string readString
      }
      
    }
    }
    
  }
 

 

 
}
