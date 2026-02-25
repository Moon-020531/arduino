int AA = 6;              
int AB = 7;  

void setup() {
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);

}

void loop() {
  digitalWrite(AA, HIGH);          //모터A를 정회전
  digitalWrite(AB, LOW);
  delay(1000);                     
 
  digitalWrite(AA, LOW);           //모터A를 정지
  digitalWrite(AB, LOW);
  delay(250);

  digitalWrite(AA, LOW);           //모터A를 역회전     
  digitalWrite(AB, HIGH);
  delay(1000);

}
