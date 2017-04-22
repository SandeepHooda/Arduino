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
char* myContacts[contactsLen]={"7837394152"}; 

#define emergencyContactsLen 2
char* emergencyContacts[emergencyContactsLen]={"9316046895","9216411835"}; 

//Locations
#define locationCount 3
double myLocationsLat[locationCount]={ 30.7275903,30.667516,30.667313};
double myLocationsLon[locationCount]={ 76.8446617, 76.885818, 76.862419}; 
char* locationNames[]={"Infosys","Home", "Gurukul"}; 
float flat, flon;
long distance ;
long nearestKnownLocationDistanceInit = 2000;
long nearestKnownLocationDistance = nearestKnownLocationDistanceInit;
String nearestKnownLocation = "";
String currentLoc = "UNKNOWN";
String lastLoc = "UNKNOWN" ;
String unknownLoc = "UNKNOWN";
boolean gpsGotLocation = false;
unsigned long smsSentTime;
int smsfrequencyMin = 30;
long safeZone = 300;
long farAwarDistance = 1000;

#define gpsLED 13//Black
#define farAway 12//Purple
#define near 11//Green
#define arrived 10//Grey 

#define emergencySwitch 2
#define rf_d_Yellow 2
#define rf_c_Orange 7
#define rf_b_Red 8
#define rf_a_Brown 9

void setup()
{

pinMode(emergencySwitch, INPUT);
pinMode(rf_d_Yellow, INPUT);
pinMode(rf_c_Orange, INPUT);
pinMode(rf_b_Red, INPUT);
pinMode(rf_a_Brown, INPUT);

 pinMode(gpsLED, OUTPUT);
  pinMode(farAway, OUTPUT);
  pinMode(near, OUTPUT);
  pinMode(arrived, OUTPUT);

  //Test
  digitalWrite(gpsLED, HIGH);
  delay(1000);
  digitalWrite(gpsLED, LOW);
  triColorLED();
  //Test

  
  
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(115200);    // Setting the baud rate of Serial Monitor (Arduino)
  delay(100);
  Serial.println("Welcome ");

  ss.begin(9600);
  delay(300);
  
 //preapreSms("hi", false);
 SendMessage("Car started", "9216411835");

}


/*void readContacts(){
  char* response;
  

   response = sendATcommandExt("AT+CPBR=1,10", "OK", 1000);   
   parseContactData(response);


    
    z//Serial.println("---response I got-- ");
   
    //Serial.println(response);
    //Serial.println("--End---");
    
}*/

void triColorLED(){
  digitalWrite(farAway, LOW);
  digitalWrite(near, LOW);
  digitalWrite(arrived, LOW);
  delay(1000);
  digitalWrite(farAway, HIGH);
  delay(1000);
  digitalWrite(farAway, LOW);
  digitalWrite(near, HIGH);
  delay(1000);
  digitalWrite(near, LOW);
  digitalWrite(arrived, HIGH);
  delay(1000);
  digitalWrite(arrived, LOW);
  delay(1000);
}

void preapreSms(String msg, boolean isEmergency){
          msg += " https://www.google.com/maps/place/@";
          msg += String(flat,6);
          msg += ",";
          msg += String(flon,6);
          msg += ",16z";
          Serial.println(msg);
          if (isEmergency){
            for (int i=0; i<emergencyContactsLen;i++){
              SendMessage(msg, emergencyContacts[i]);
            }
          }else {
            for (int i=0; i<contactsLen;i++){
              SendMessage(msg, myContacts[i]);
            }
          }
          
          smsSentTime = millis();
          triColorLED();
          Serial.println("SMS sent");
}


void getMeNearestLocation(){
  nearestKnownLocationDistance = nearestKnownLocationDistanceInit; //Rest this value
  nearestKnownLocation = "";
  for(int i=0;i<locationCount;i++){
           distance = (unsigned long)TinyGPS::distance_between(flat, flon, myLocationsLat[i], myLocationsLon[i]);
          
          if (distance < nearestKnownLocationDistance){
            nearestKnownLocationDistance = distance;
            nearestKnownLocation = locationNames[i];
          }
          if (distance <= safeZone ){
            currentLoc = locationNames[i];
            break;
          }else {
            currentLoc = unknownLoc;
          }
        } //Loop thru all the known locatios End
  
}
void loop(){
  
   if(digitalRead(emergencySwitch)){
    Serial.println("I am in danger. Please help");
     preapreSms("I am in danger. Please help",true);
      delay(500);
      
   }
 /* if (digitalRead(rf_d_Yellow)){
    delay(500);
     Serial.println("Sms from Car GPS ");
    //SendMessage("Sms from Car GPS", "9216411835");
    
  }

  if (digitalRead(rf_c_Orange) || digitalRead(rf_b_Red) || digitalRead(rf_a_Brown)){
    delay(500);
    Serial.println("I am in danger. Please help");
     //preapreSms("I am in danger. Please help",true);
 }*/
 
  loopForGPS();
  if (flat != TinyGPS::GPS_INVALID_F_ANGLE){

        digitalWrite(gpsLED, HIGH);
        if(!gpsGotLocation){//Send one sms as soon as GPS gets location on power on
          getMeNearestLocation();
          preapreSms("Starting from "+nearestKnownLocation, false);
          gpsGotLocation = true;
        }else {
          if((millis() - smsSentTime) > (60000 * smsfrequencyMin) ){
            
            preapreSms(" Periodic update: I am at", false);
          }
        }
       
       
       
        Serial.println("---Location -- ");
        print_float(flat,0,10,6);
        Serial.print(" ");
        print_float(flon,0,10,6);
        Serial.println(" ");
        Serial.println("---End-- ");
        
        getMeNearestLocation();

        if((millis() - smsSentTime) > 60000  ){ //Don't sent SMS for next 1 minute
          if (!unknownLoc.equals( currentLoc ) && unknownLoc.equals( lastLoc)){
              String msg = "Entering ";
              msg += currentLoc;
              Serial.println(msg);
              preapreSms(msg, false);
          }else if ( unknownLoc.equals( currentLoc ) && !unknownLoc.equals( lastLoc) ){
            
            String msg = "Exiting ";
            msg += nearestKnownLocation;// lastLoc;
            Serial.println(msg);
            preapreSms(msg, false);
            
          }
        }
          
         
         if (nearestKnownLocationDistance > safeZone && nearestKnownLocationDistance <= farAwarDistance){
            digitalWrite(farAway, LOW);
            digitalWrite(near, HIGH);
            digitalWrite(arrived, LOW);
            
         } else if ( (nearestKnownLocationDistance > farAwarDistance) || unknownLoc.equals( currentLoc )){
            digitalWrite(farAway, HIGH);
            digitalWrite(near, LOW);
            digitalWrite(arrived, LOW);
         }else if (nearestKnownLocationDistance <= safeZone){
            digitalWrite(farAway, LOW);
            digitalWrite(near, LOW);
            digitalWrite(arrived, HIGH);
         }
          
      // preapreSms(" I am staying at ", false);
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
