/*
 * TemperatureRaw
 * 
 * Leser av temperaturen ved hjelp av analog temperaturssensor LM35.
 * Resultatet skrives ut til serielvindu.
 * 
 * Kretsen:
 *   1 x LM35 analog temperatursensor
 * Oppkobling vises på wiki:
 * https://www.ntnu.no/wiki/display/plab/7.+LM35+Temperatursensor
 * 
 * 
 * Reads the temperature with the analogue sensor LM35.
 * The results are printed to serial output.
 * 
 * Circuit:
 *   1 x LM35 analogue temperature sensor
 * How to connect curcuit can be seen here:
 * https://www.ntnu.no/wiki/display/plab/7.+LM35+Temperatursensor
 */

const int sensorPin = A3; // the analog pin the LM35's Vout (sense) pin is connected to
                          // the resolution is 10 mV / degree centigrade
void setup()
{
  Serial.begin(9600);  // Start the serial connection with the computer
                       // to view the result open the serial monitor 
}
 
void loop()                     // run over and over again
{
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);  
  // converting that reading to voltage
  float voltage = reading * 5.0 / 1024; 
  // print out the voltage
  Serial.print(voltage);
  Serial.println(" volts");
  // now print out the temperature
  float temperatureC = (voltage) * 100 + 2 ; // converting from 10 mv per degree with 2 degrees offset
  Serial.print(temperatureC);
  Serial.println(" degrees C");
  delay(1000); //waiting a second
}
