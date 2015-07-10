/*
  PortExpander_HelloWorld
    Modified from: LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The PLabLiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver running in 4 bit mode. There are many of
 them out there, and you can usually tell them by the 16-pin
 interface.

 This sketch prints "Hello World!" to the LCD and shows the time.
 In addition it blinks a LED connected to the port expander.

  The circuit:
 * LCD RS pin to MCP23008 GP5 (pin 15)
 * LCD Enable pin to MCP23008 GP4 (pin 14)
 * LCD D4 pin to MCP23008 GP3 (pin 13)
 * LCD D5 pin to MCP23008 GP2 (pin 12)
 * LCD D6 pin to MCP23008 GP1 (pin 11)
 * LCD D7 pin to MCP23008 GP0 (pin 10)
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * LED to GP6 (pin 16) through 220 ohm resistor to ground
See:
https://www.ntnu.no/wiki/display/plab/4.+1602A+LCD+display

 example created 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified and added to PLabExamples 10 Jul 2015
 by Inge Edward Halsaunet

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library dependencies code:
#include <Wire.h>                  // MCP23008 code uses I2C
#include <Adafruit_MCP23008.h>     // MCP23008 library
// include the library code:
#include <PLabLiquidCrystal.h>

// I2C address of port expander. This address won't change
const int i2c_address = 0;

// Pin addresses for LCD display. These won't change
const int rs_addr = 5;
const int enable_addr = 4;
const int d4_addr = 3;
const int d5_addr = 2;
const int d6_addr = 1;
const int d7_addr = 0;

const int led_addr = 6;

// initialize the library with the numbers of the interface pins
PLabLiquidCrystal lcd(rs_addr, enable_addr,
                      d4_addr, d5_addr, d6_addr, d7_addr,
                      i2c_address);

void setup() {
  // set up the LCD's number of columns and rows:
  // Begin MUST be called before anything can be printed on display
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  lcd.pinMode(led_addr, OUTPUT);
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);

  // if seconds since start is an odd number
  if (((millis() / 1000) % 2) == 1) {
    // Turn on LED
    lcd.digitalWrite(led_addr, HIGH);
  } else {
    // In even seconds, turn off led
    lcd.digitalWrite(led_addr, LOW);
  }
}

