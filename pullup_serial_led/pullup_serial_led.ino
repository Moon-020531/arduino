const int ledPin=12;
const int YledPin=13;
const int buttonPin=2;
int sensorPin = A0;
int sensorValue = 0;  
int buttonState =0;


void setup() {

  pinMode(ledPin, OUTPUT);
   pinMode(YledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin);
  sensorValue = analogRead(sensorPin);
  Serial.println(buttonState);

  if(buttonState == HIGH){
   digitalWrite(ledPin, HIGH);
    digitalWrite(YledPin, LOW);
  //  Serial.println("LED ON");
   delay(sensorValue/2);

   digitalWrite(ledPin, LOW);
    digitalWrite(YledPin, HIGH);
    delay(sensorValue/2);

  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(YledPin, LOW);
    
    delay(sensorValue/2);
  }




//  digitalWrite(ledPin, HIGH);
//  delay(500);

// digitalWrite(ledPin, LOW);
//  delay(500);   
}
