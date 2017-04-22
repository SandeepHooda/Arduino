const int buttonPin4 = 4;     // the number of the pushbutton pin
const int buttonPin5 = 5;     // the number of the pushbutton pin
const int buttonPin6 = 6;     // the number of the pushbutton pin
const int buttonPin7 = 7;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  pinMode(buttonPin7, INPUT);
  
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin4);
  if(digitalRead(buttonPin4)){
    Serial.println(4);
    delay(500);
  }
   if(digitalRead(buttonPin5)){
    Serial.println(5);
    delay(500);
  }
   if(digitalRead(buttonPin6)){
    Serial.println(6);
    delay(500);
  } if(digitalRead(buttonPin7)){
    Serial.println(7);
    delay(500);
  }

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
  } else {
    // turn LED off:
    //digitalWrite(ledPin, LOW);
  }
}
