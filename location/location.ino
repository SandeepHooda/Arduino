/*********************************************************************
  This is the code for Fetching your location from Google Geolocation API
  
  This code is provided by 
  techiesms
*********************************************************************/

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "ESP8266WiFi.h"

//char myssid[] = "BASEMENT";         // your network SSID (name)
//char mypass[] = "A0A0A0A0A0";          // your network password
char myssid[] = "Sandeep";         // your network SSID (name)
char mypass[] = "1234567890";          // your network password

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = ""


const char* HostAppSpot = "idonotremember-app.appspot.com";
String thisPageAppSpot = "/ws/location/getAddress";
String healthUpdate = "/ws/location/healthPing/wifii/";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";
String wifii = "";

double latitude    = 0.0;
double longitude   = 0.0;
double accuracy    = 0.0;
int more_text = 1; // set to 1 for more debug output


void connect(){
   WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(myssid);
  WiFi.begin(myssid, mypass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");
}

void setup()   {
  Serial.begin(9600);


  Serial.println("Start");
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
 connect();


}
void updateHealth(){

 
 WiFiClientSecure client;
 Serial.print("updateHealth: ");
 
  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)HostAppSpot + healthUpdate+wifii );
  Serial.println(" ");
  if (client.connect(HostAppSpot, 443)) {
    Serial.println("Connected");
    client.println("GET " + healthUpdate +wifii +" HTTP/1.1");
    client.println("Host: " + (String)HostAppSpot);
    client.println("Connection: close\r\n\r\n");
   
    delay(500);
  }

  //Read and parse all the lines of the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    //if (more_text) {
      Serial.print(line);
   // }
 
  }

  Serial.println("closing connection to health ping");
  Serial.println();
  client.stop();
   
}
void getAddress(double lat, double lan, double accuracy, int nwCount){
 jsonString = "{\n";
 jsonString += "\"wifii\": \""+wifii+"\",\n";
 jsonString += "\"nwCount\": \""+String(nwCount)+"\",\n";
 jsonString += "\"accuracy\": \""+String(accuracy,6)+"\",\n";
 jsonString += "\"latitude\": \""+String(lat,6)+"\",\n";
 jsonString += "\"longitude\": \""+String(lan,6)+"\"\n";  
  
 jsonString += ("}\n");
  
  WiFiClientSecure client;
  Serial.print("data to app spot: ");
Serial.print(jsonString);
  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)HostAppSpot + thisPageAppSpot );
  Serial.println(" ");
  if (client.connect(HostAppSpot, 443)) {
    Serial.println("Connected");
    client.println("POST " + thisPageAppSpot + " HTTP/1.1");
    client.println("Host: " + (String)HostAppSpot);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.print(jsonString);
    delay(500);
  }

  //Read and parse all the lines of the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    //if (more_text) {
      Serial.print(line);
   // }
 
  }

  Serial.println("#closing connection to app spot get address");
  Serial.println();
  client.stop();
  delay(5000);

}
void getPosionBasedOnNetworks (int nwCount){
  Serial.println("");
 DynamicJsonBuffer jsonBuffer;
  WiFiClientSecure client;

  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)Host + thisPage + "AIzaSyCYNXIYINPmTNIdusMjJloS4_BXSOff1_g");
  Serial.println(" ");
  if (client.connect(Host, 443)) {
    Serial.println("Connected");
    client.println("POST " + thisPage + key + " HTTP/1.1");
    client.println("Host: " + (String)Host);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.print(jsonString);
    delay(500);
  }

  //Read and parse all the lines of the reply from server
  boolean latLanFound = false;
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if (more_text) {
      Serial.print(line);
    }
    JsonObject& root = jsonBuffer.parseObject(line);
    if (root.success()) {
      latitude    = root["location"]["lat"];
      longitude   = root["location"]["lng"];
      accuracy   = root["accuracy"];
      latLanFound = true;
    }else {
     
    }
  }

  Serial.println("closing connection");
  Serial.println();
  client.stop();

  Serial.print("Latitude = ");
  Serial.println(latitude, 6);
  Serial.print("Longitude = ");
  Serial.println(longitude, 6);
  Serial.print("Accuracy = ");
  Serial.println(accuracy);
  if (latitude != 0.0 ){
    Serial.println("Going to get address");
    getAddress(latitude,longitude,accuracy, nwCount);
  }else {
    Serial.println("updating health");
    updateHealth();
    delay(500);
  }

}
void loop() {
wifii = "";
 if (WiFi.status() != WL_CONNECTED){
  connect();
 }
  char bssid[6];
  
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
   
    Serial.print(n);
    Serial.println(" networks found...");

    if (more_text) {
      // Print out the formatted json...
      Serial.println("{");
      Serial.println("\"homeMobileCountryCode\": 404,");  // this is a real India MCC
      Serial.println("\"homeMobileNetworkCode\": 88,");   // and a real India vodaphone punjab MNC
      Serial.println("\"radioType\": \"gsm\",");          // for gsm
      Serial.println("\"carrier\": \"Vodafone\",");       // associated with Vodafone
      Serial.println("\"cellTowers\": [");                // I'm not reporting any cell towers
      Serial.println("],");
      Serial.println("\"wifiAccessPoints\": [");
      for (int i = 0; i < n; ++i)
      {
        wifii += WiFi.SSID(i)+",";
        Serial.println("{");
        Serial.print("\"macAddress\" : \"");
        Serial.print(WiFi.BSSIDstr(i));
        Serial.println("\",");
         Serial.print("\"SSID\" : \"");
        Serial.print(WiFi.SSID(i));
        Serial.println("\",");
        Serial.print("\"signalStrength\": ");
        Serial.println(WiFi.RSSI(i));
        if (i < n - 1)
        {
          Serial.println("},");
        }
        else
        {
          Serial.println("}");
        }
      }
      Serial.println("]");
      Serial.println("}");
    }
    Serial.println(" ");
  }
  // now build the jsonString...
  jsonString = "{\n";
  jsonString += "\"homeMobileCountryCode\": 404,\n"; // this is a real India  MCC
  jsonString += "\"homeMobileNetworkCode\": 88,\n";  // and a real India vodaphone punjab MNC
  jsonString += "\"radioType\": \"gsm\",\n";         // for gsm
  jsonString += "\"carrier\": \"Vodafone\",\n";      // associated with Vodafone
  jsonString += "\"considerIp\": \"false\",\n";      // consider IP
  jsonString += "\"wifiAccessPoints\": [\n";
  for (int j = 0; j < n; ++j)
  {
    jsonString += "{\n";
    jsonString += "\"macAddress\" : \"";
    jsonString += (WiFi.BSSIDstr(j));
    jsonString += "\",\n";
    jsonString += "\"signalStrength\": ";
    jsonString += WiFi.RSSI(j);
    jsonString += "\n";
    if (j < n - 1)
    {
      jsonString += "},\n";
    }
    else
    {
      jsonString += "}\n";
    }
  }
  jsonString += ("]\n");
  jsonString += ("}\n");
  //--------------------------------------------------------------------

if (n > 0){
  getPosionBasedOnNetworks(n);
  delay(5000);
}// If we have few networks 




}


