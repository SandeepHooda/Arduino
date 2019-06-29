//https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Filters.h> //Easy library to do the RMS calculations
const char* ssid = "Sandeep";
const char* password = "1234567890";
float testFrequency = 50;                     // test signal frequency (Hz)
float windowLength = 40.0/testFrequency;     // how long to average the signal, for statistist

int Sensor = 0; //Sensor analog input, here it's A0

float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float current_Volts; // Voltage

unsigned long printPeriod = 1000; //Refresh rate
unsigned long previousMillis = 0;

RunningStatistics inputStats;                //Easy life lines, actual calculation of the RMS requires a load of coding
String host     = "http://voltage-logger.herokuapp.com/VoltageLogger?voltage="; // Your domain 

void setup () {
 
Serial.begin(115200);
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) {
 
delay(1000);
Serial.println("Connecting..");
 
}
 inputStats.setWindowSecs( windowLength );
}
 
void loop() {
  //Read voltage
  Sensor = analogRead(A0);  // read the analog in value:
  inputStats.input(Sensor);  // log to Stats function
  current_Volts = intercept + slope * inputStats.sigma(); //Calibartions for offset and amplitude
  current_Volts= current_Volts*(40.3231);                //Further calibrations for the amplitude
 
if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 String url = host+current_Volts;
HTTPClient http;  //Declare an object of class HTTPClient
 
http.begin(url);  //Specify request destination
int httpCode = http.GET();                                                                  //Send the request
 
if (httpCode > 0) { //Check the returning code
 
String payload = http.getString();   //Get the request response payload
Serial.println(payload);                     //Print the response payload
 
}
 
http.end();   //Close connection
 
}
 
delay(1000);    //Send a request every 30 seconds
 
}
