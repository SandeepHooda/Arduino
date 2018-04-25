/*
  Web client

  This sketch connects to a website (http://www.google.com)
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 18 Dec 2009
  by David A. Mellis
  modified 9 Apr 2012
  by Tom Igoe, based on work by Adrian McEwen

*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "192.168.1.6";    // name address for Google (using DNS)
char serverHeroku[] = "post-master.herokuapp.com";    // name address for Google (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
EthernetClient clientEmail;

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
connectToCamera();
 
}
void connectToCamera(){
   // if you get a connection, report back via serial:
  if (client.connect(server, 8090)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /snapshot.cgi HTTP/1.1");
    client.println("Authorization: Digest username=\"sandeephooda\", realm=\"ipcamera_00626E473AAE\", nonce=\"79f17c7a2caabb6a7ca1a6910225c791\", uri=\"/snapshot.cgi\", algorithm=MD5, response=\"0a442fe97d3bbe01fff12bb2518a113b\", qop=auth, nc=00000003, cnonce=\"3d18f59d8b24e0b8\"");
    client.println("Host: 192.168.1.6");
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
    Serial.println("connection failed");
  }
}
void captureLoop() {
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

delay(600000);
    // do nothing forevermore:
    connectToCamera();
  }
}

void loop() {
  captureLoop();
}

