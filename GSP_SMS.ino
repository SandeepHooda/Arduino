 #include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial mySerial(5, 6);//rx, tx 
//5 rx of arduinto -> tx of module
//6 tx of arduinto -> rx of module
int8_t answer;

//GPS

TinyGPS gps;
SoftwareSerial ss(4, 3);//rx ( of ard, ), tx ( of ard)

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

#define contactsLen 1
char* myContacts[contactsLen]={"9216411835"}; 

//Locations
#define locationCount 2
double myLocationsLat[locationCount]={ 30.7275903,30.667516};
double myLocationsLon[locationCount]={ 76.8446617, 76.885818}; 
char* locationNames[]={"Infosys","Home"}; 
float flat, flon;
long distance ;
long minDistance = 9999999999;
String currentLoc = "";
String lastLoc = "" ;
String unknownLoc = "UNKNOWN";
boolean gpsGotLocation = false;
unsigned long smsSentTime;
int smsfrequencyMin = 30;
int safeZone = 300;

#define gpsLED 13//Black
#define farAway 12//Purple
#define near 11//Green
#define arrived 10//Grey
#define testpoint 8
void setup()
{

pinMode(testpoint, OUTPUT);
digitalWrite(testpoint, HIGH);
 pinMode(gpsLED, OUTPUT);
  pinMode(farAway, OUTPUT);
  pinMode(near, OUTPUT);
  pinMode(arrived, OUTPUT);

  //Test
  digitalWrite(gpsLED, HIGH);
  delay(1000);
  digitalWrite(gpsLED, LOW);
  digitalWrite(farAway, HIGH);
  delay(1000);
  digitalWrite(farAway, LOW);
  digitalWrite(near, HIGH);
  delay(1000);
  digitalWrite(near, LOW);
  digitalWrite(arrived, HIGH);
  delay(1000);
  digitalWrite(arrived, LOW);
  //Test

  
  
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(115200);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.println("Welcome ");

  ss.begin(9600);
  delay(300);
  
 //preapreSms("hi");
 

}


/*void readContacts(){
  char* response;
  

   response = sendATcommandExt("AT+CPBR=1,10", "OK", 1000);   
   parseContactData(response);


    
    z//Serial.println("---response I got-- ");
   
    //Serial.println(response);
    //Serial.println("--End---");
    
}*/

void preapreSms(String msg){
          msg += " https://www.google.com/maps/place/@";
          msg += String(flat,6);
          msg += ",";
          msg += String(flon,6);
          msg += ",16z";
          Serial.println(msg);
          for (int i=0; i<contactsLen;i++){
            SendMessage(msg, myContacts[i]);
          }
          smsSentTime = millis();
          Serial.println("SMS sent");
}

void loop(){
  loopForGPS();
  if (flat != TinyGPS::GPS_INVALID_F_ANGLE){

        digitalWrite(gpsLED, HIGH);
        if(!gpsGotLocation){//Send one sms as soon as GPS gets location on power on
          preapreSms("Starting from");
          gpsGotLocation = true;
        }else {
          if((millis() - smsSentTime) > (60000 * smsfrequencyMin) ){
            
            preapreSms(" Periodic update: I am at");
          }
        }
       
       
       
        Serial.println("---Location -- ");
        print_float(flat,0,10,6);
        Serial.print(" ");
        print_float(flon,0,10,6);
        Serial.println(" ");
        Serial.println("---End-- ");
        for(int i=0;i<locationCount;i++){
           distance = (unsigned long)TinyGPS::distance_between(flat, flon, myLocationsLat[i], myLocationsLon[i]);
          Serial.print(locationNames[i]);
          Serial.println(distance);
          if (distance < minDistance){
            minDistance = distance;
          }
          if (distance <= safeZone ){
            currentLoc = locationNames[i];
            break;
          }else {
            currentLoc = unknownLoc;
          }
        }
        
          if (!unknownLoc.equals( currentLoc ) && unknownLoc.equals( lastLoc)){
              String msg = "Entering ";
             msg += currentLoc;
             Serial.println(msg);
             preapreSms(msg);
            delay(120000 );
            
          }else if ( unknownLoc.equals( currentLoc ) && !unknownLoc.equals( lastLoc) ){
            
            String msg = "Exiting ";
            msg += lastLoc;
           Serial.println(msg);
           preapreSms(msg);
            delay(120000 );
          }

         if (minDistance > safeZone && minDistance <= 2000){
            digitalWrite(farAway, LOW);
            digitalWrite(near, HIGH);
            digitalWrite(arrived, LOW);
            
         } else if (minDistance > 2000){
            digitalWrite(farAway, HIGH);
            digitalWrite(near, LOW);
            digitalWrite(arrived, LOW);
         }else if (minDistance <= 200){
            digitalWrite(farAway, LOW);
            digitalWrite(near, LOW);
            digitalWrite(arrived, HIGH);
         }
          
      // preapreSms(" I am staying at ");
        lastLoc = currentLoc;
       // print_int( distance, 0xFFFFFFFF, 9);
    
  }else {
    digitalWrite(gpsLED, LOW);
    Serial.println("GPS didn't get location yet");
  }
 
}


 void SendMessage( String text, String phoneNo)
{
  
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  String phoneNoCmd = "AT+CMGS=\"";
  phoneNoCmd += phoneNo;
  phoneNoCmd += "\"";
  mySerial.println(phoneNoCmd); // Replace x with mobile number
  delay(1000);
  mySerial.println(text);// The SMS text you want to send
  delay(500);
 mySerial.write(0x1A);  // sends ctrl+z end of message
   mySerial.write(0x0D);  // Carriage Return in Hex
mySerial.write(0x0A);  // Line feed in Hex
//The 0D0A pair of characters is the signal for the end of a line and beginning of another.
delay(5000);
  delay(500);
}



 


void setupGPS()
{

  Serial.print("Testing TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("by Mikal Hart");
  Serial.println();
  Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  Serial.println("-------------------------------------------------------------------------------------------------------------------------------------"); 
}


void loopForGPS()
{
  
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  
  String buf;
  buf += F("your location is \nlat:");
  buf += String(flat, 6);
  buf += F("\nlong:");
  buf += String(flon, 6);
   //SendMessage(buf);
   Serial.println(buf);
  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  print_date(gps);
  print_float(gps.f_altitude(), TinyGPS::GPS_INVALID_F_ALTITUDE, 7, 2);
  print_float(gps.f_course(), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_float(gps.f_speed_kmph(), TinyGPS::GPS_INVALID_F_SPEED, 6, 2);
  print_str(gps.f_course() == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(gps.f_course()), 6);
  print_int(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0xFFFFFFFF : (unsigned long)TinyGPS::distance_between(flat, flon, LONDON_LAT, LONDON_LON) / 1000, 0xFFFFFFFF, 9);
  print_float(flat == TinyGPS::GPS_INVALID_F_ANGLE ? TinyGPS::GPS_INVALID_F_ANGLE : TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON), TinyGPS::GPS_INVALID_F_ANGLE, 7, 2);
  print_str(flat == TinyGPS::GPS_INVALID_F_ANGLE ? "*** " : TinyGPS::cardinal(TinyGPS::course_to(flat, flon, LONDON_LAT, LONDON_LON)), 6);

  gps.stats(&chars, &sentences, &failed);
  print_int(chars, 0xFFFFFFFF, 6);
  print_int(sentences, 0xFFFFFFFF, 10);
  print_int(failed, 0xFFFFFFFF, 9);
  Serial.println();
  
  smartdelay(1000);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartdelay(0);
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("********** ******** ");
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ",
        month, day, year, hour, minute, second);
    Serial.print(sz);
  }
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  smartdelay(0);
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartdelay(0);
}
