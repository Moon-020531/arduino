
#include <EduIntro.h>

DHT11 dht11(D7);  // creating the object sensor on pin 'D7'

int C;   // temperature C readings are integers
//  int F// temperature F readings are returned in float format
int H;   // humidity readings are integers

void setup()
{
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
}

void loop()
{
  dht11.update();

  C = dht11.readCelsius();     	// Reading the temperature in Celsius degrees and store in the C variable
  // F = dht11.readFahrenheit();  	// Reading the temperature in Fahrenheit degrees and store in the F variable
  H = dht11.readHumidity();     // Reading the humidity index

  // Print the collected data in a row on the Serial Monitor
  Serial.print(H);
   Serial.print(",");
  Serial.println(C);
  // Serial.print("\tF: ");
  // Serial.println(F);

  delay(2000);                // Wait one second before get another temperature reading
}
