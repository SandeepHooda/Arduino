#include <ESP8266WiFi.h>

#define motion_sensor  D0
#define light_sensor  A0
#define relay_control  D5

int oneMinute = 60*1000;

int second = 1000;
int monitoringTimer = 0;
int  lightIntensity =0;
int motionSensor_var;

// Web Server specific code
const char* ssid     = "GREEN_2.4";
const char* password = "A0A0A0A0A0";
// Set web server port number to 80
WiFiServer server(80);
// Variable to store the HTTP request
String header;
// Auxiliar variables to store the current output state
String smartMode = "on";
String forceMode = "on";
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Set your Static IP address
IPAddress local_IP(192, 168, 0, 199);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional
// End Web Server specific code
void setup(){
  pinMode (motion_sensor,INPUT);
  pinMode (relay_control, OUTPUT);
   if  ( isNight()   )   {
    digitalWrite (relay_control,HIGH);
   }else {
    digitalWrite (relay_control,LOW);
   }
  
  Serial.begin(9600); 
   delay(200);
  Serial.println("Welcome"); 
  //delay(60000);
  Serial.println("let us start"); 

  // Connect to Wi-Fi network with SSID and password
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

boolean isMotion(){
   motionSensor_var = digitalRead(motion_sensor);
  //Serial.print("IS motion ");
 //Serial.println(motionSensor_var); 
  if (motionSensor_var == HIGH){
    return true;
  }else {
    return false;
  }
}
boolean isNight(){
 lightIntensity =  analogRead(light_sensor);
 //Serial.print("Light intensity ");
 //Serial.println(lightIntensity); 
 if (lightIntensity < 300){
    return true;
  }else {
    return false;
  }
}


void runWebServer(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /smartMode/on") >= 0) {
             smartMode = "on";
            } else if (header.indexOf("GET /smartMode/off") >= 0) {
             smartMode = "off";
            }else if (header.indexOf("GET /forceMode/on") >= 0) {
             forceMode = "on";
            } else if (header.indexOf("GET /forceMode/off") >= 0) {
             forceMode = "off";
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>Shaurya's Smart lantern</h1>");
            
            
            
            isNight();
            isMotion();
           client.println("<p>lightIntensity " + String(lightIntensity)+ "." +String(random(300))+ "</p>");
            client.println("<p>Motion " + String(motionSensor_var) + "</p>");
            client.println("<p>Smart Mode " + smartMode + "</p>");
            // If the output5State is off, it displays the ON button       
            if (smartMode=="off") {
              client.println("<p><a href=\"/smartMode/on\"><button class=\"button\">ON</button></a></p>");
               client.println("<p>Manual Mode " + forceMode + "</p>");
              if (forceMode == "off"){
                client.println("<p><a href=\"/forceMode/on\"><button class=\"button\">ON</button></a></p>");
              }else {
                client.println("<p><a href=\"/forceMode/off\"><button class=\"button\">OFF</button></a></p>");
              }
            } else {
              client.println("<p><a href=\"/smartMode/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
         
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
void loop (){
  
  delay(100); //this delay is to let the sensor settle down before taking a reading
  runWebServer();
if (smartMode=="on") {
      if (isMotion()){// motion yes
      //Serial.println("There is motion");   
      if  ( isNight()   )   {//Night yes
          digitalWrite (relay_control,HIGH);
          monitoringTimer = oneMinute;//Monitor timer 
          while(monitoringTimer >0 ){//runs for N minutes and sencr motion withinh this period
            // isNight();
            monitoringTimer = monitoringTimer -second;
            //Serial.print("Timer left ");
             //Serial.println(monitoringTimer);
            delay(second);
            runWebServer();
             if (isMotion()){// motion yes 
              monitoringTimer = oneMinute; 
             }
             if (smartMode=="off") {
              monitoringTimer =0;
             }
          }
       }else {//Night no
         digitalWrite (relay_control,LOW);
         }
      }else {// No motion 
         digitalWrite (relay_control,LOW);
         
      }
  
}else {
  if (forceMode =="on"){
     digitalWrite (relay_control,HIGH);//Smart mode off
  }else {
     digitalWrite (relay_control,LOW);//Smart mode off
  }
 
}
  
  
}

