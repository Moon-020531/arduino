const int ledPin=13;
const int buttonPin=2;

int buttonState =0;


void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
   digitalWrite(ledPin, LOW);
  delay(500);
  } else {
    
    digitalWrite(ledPin, HIGH);
    delay(500);
  }




//  digitalWrite(ledPin, HIGH);
//  delay(500);

// digitalWrite(ledPin, LOW);
//  delay(500);   
}
