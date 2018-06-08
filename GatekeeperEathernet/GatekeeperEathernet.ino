

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "192.168.225.21";    // name address for IP camera
char serverHeroku[] = "post-master.herokuapp.com";    // name address for Google (using DNS)
char serverHerokuMdb[] ="remind-me-on-heroku.herokuapp.com";
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
EthernetClient clientEmail;
EthernetClient clientMdb;
String MdbResponse ="";
boolean motionDetected = false;
boolean emailSent = false;

int pos = 0;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  connectToMdb();
 
}
void connectToMdb(){
  if (clientMdb.connect(serverHerokuMdb, 80)) {
    MdbResponse = "";
    Serial.println("connected to MDB");
    // Make a HTTP request:
    clientMdb.println("GET /GetMotionStatus HTTP/1.1");
    clientMdb.println("Host: remind-me-on-heroku.herokuapp.com");
    clientMdb.println("Connection: close");
    clientMdb.println();
    Serial.println("is client available");
  }
}

void readMdbData(){
  
  if (clientMdb.available()){
       char c = clientMdb.read();
       MdbResponse += c;
       //Serial.print(c);
  }
  if (!clientMdb.connected()) {
    Serial.print(MdbResponse);
     Serial.println("disconnecting mango DB.");
     clientMdb.stop();

     if (MdbResponse.indexOf("\"motionDetected\" : \"true\"") >=0 ){
      motionDetected = true;
     }else {
      motionDetected = false;
     }
       delay(1000);
      
     connectToMdb();
   }
    
    if (motionDetected == true){
        
        connectToCamera();
        while (emailSent == false && motionDetected == true){
           captureImage();
        }
       }
}

void connectToCamera(){
   // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    pos = 0;
     emailSent = false;
    Serial.println("connected ot foscam");
    // Make a HTTP request:
    client.println("GET /snapshot.cgi HTTP/1.1");
    client.println("Authorization: Digest username=\"sandeephooda\", realm=\"ipcamera_00626E473AAE\", nonce=\"79f17c7a2caabb6a7ca1a6910225c791\", uri=\"/snapshot.cgi\", algorithm=MD5, response=\"0a442fe97d3bbe01fff12bb2518a113b\", qop=auth, nc=00000003, cnonce=\"3d18f59d8b24e0b8\"");
    client.println("Host: 192.168.225.21");
    client.println("Connection: close");
    client.println();

    if (clientEmail.connect(serverHeroku, 80)) {

      clientEmail.println("POST /FoscamImage HTTP/1.1");
      // clientEmail.println("Authorization: Digest username=\"sandeephooda\", realm=\"ipcamera_00626E473AAE\", nonce=\"79f17c7a2caabb6a7ca1a6910225c791\", uri=\"/snapshot.cgi\", algorithm=MD5, response=\"0a442fe97d3bbe01fff12bb2518a113b\", qop=auth, nc=00000003, cnonce=\"3d18f59d8b24e0b8\"");
      clientEmail.println("Host: post-master.herokuapp.com");
      clientEmail.println("Content-Type: application/octet-stream");
      clientEmail.println("Content-Length: 30000");
      //clientEmail.println("Connection: close");
      clientEmail.println();
      
     
    }

  } else {
    // if you didn't get a connection to the server:
    Serial.println("Not able to connect to foscam connection failed");
     client.stop();
    clientEmail.stop();
    delay(1000);
    emailSent = true;
  }
}
void captureImage() {
  // if there are incoming bytes available
  // from the server, read them and print them:

  if (client.available()) {
    char c = client.read();

    clientEmail.print(c);

    pos++;
    //Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println(pos);
    Serial.println("disconnecting.");
    client.stop();
    clientEmail.stop();
    emailSent = true;
    motionDetected = false ;
    Serial.println("disconneced from foscam.");
    delay(1000);
    // do nothing forevermore:
   // connectToCamera();
  }
}

void loop() {
  //check Gatekeep Server if motion is detected http://192.168.225.32 = 1 then capture loop 
  //After sending email make EmailSentConfirmation to http://192.168.225.32
  readMdbData();
 
}

