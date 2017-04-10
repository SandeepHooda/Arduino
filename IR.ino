


#define outputPin 2
boolean objectDetect;
void setup()
{
  pinMode(outputPin, INPUT);


  Serial.begin(9600); 
  // prints title with ending line break 
  Serial.println("Program Starting"); 
  // wait for the long string to be sent 
  delay(100); 
}

void loop()
{  
  
    delay(100);
    if( digitalRead( outputPin))    // If the Output is now HIGH,
    {                               // then first Read was noise
        objectDetect = false;       // and no object was detected;
    }
    else                            // but if the Output is still LOW,
    {
        objectDetect = true;        // then an object was truly detected.
        Serial.println("Object detected");
    }
}


