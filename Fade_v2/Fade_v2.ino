int led = 9;         // the PWM pin the LED is attached to
int brightness = 0;  // how bright the LED is
int fadeAmount =3;  // how many points to fade the LED by
int val = 0;
int analogPin = A3;
int ledPin = 9; 


// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  val = analogRead(analogPin);  // read the input pin
  analogWrite(ledPin, val / 4);
  // analogWrite(led, brightness);
  
  //   Serial.println(voltage);
  // // change the brightness for next time through the loop:
  // brightness = brightness + fadeAmount;

  // // reverse the direction of the fading at the ends of the fade:
  // if (brightness <= 0 || brightness >= 255) {
  //   fadeAmount = -fadeAmount;
    

  // }
  // wait for 30 milliseconds to see the dimming effect
  // delay(30);
  Serial.println(val);
  
}
