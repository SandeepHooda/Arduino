#include <SoftwareSerial.h>;
#include <TinyGPS.h>;
SoftwareSerial mySerial(10, 11); // RX, TX
TinyGPS gps; // create gps object
long lat, lon; // create variable for latitude and longitude object
void setup() {
// Open serial communications and wait for port to open:
Serial.begin(57600);
while (!Serial) ; // wait for serial port to connect. Needed for native USB port only
Serial.println("TIF LABS");
mySerial.begin(9600); // set the data rate for the SoftwareSerial port
mySerial.println("ABHINANDAN");
}
void loop() {
// run over and over
if (mySerial.available()) {
char c = mySerial.read();
Serial.write( c );
if (gps.encode( c )) {
// encode gps data

gps.get_position(&lat,&lon); // get latitude and longitude
Serial.print("Position: "); // display position
Serial.print("Lat: ");
Serial.print(lat); // print latitude
Serial.print(" Lon: ");
Serial.println(lon); // print longitude
}
}
}
