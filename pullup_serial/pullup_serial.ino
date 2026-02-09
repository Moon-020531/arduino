const int ledPin=12;
const int buttonPin=2;

int buttonState =0;


void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  if(buttonState == HIGH){
   digitalWrite(ledPin, HIGH);
   Serial.println("LED ON");
    delay(500);

  } else {
    digitalWrite(ledPin, LOW);
    delay(500);
  }




//  digitalWrite(ledPin, HIGH);
//  delay(500);

// digitalWrite(ledPin, LOW);
//  delay(500);   
}
