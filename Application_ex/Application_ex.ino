#include <Servo.h>

Servo myservo;  

int potpin = A3;  
int val;    
int green=13;
int red=12;

void setup() {
  myservo.attach(9);  
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
}

void loop() {
  val = analogRead(potpin);            
  val = map(val, 0, 1023, 0, 180);
  if(val>=30&&val<=150){
    digitalWrite(green, HIGH);
     digitalWrite(red, LOW);
  }else{
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
  }
  myservo.write(val);                
  delay(15);                           
}
