/*
 * Check
 * 
 * Leser RFID kort og sammenligner med to kjente kort.
 * 
 * Kretsen:
 *   1 x RFID RC522
 * Mer om RFID leseren kan du finne her:
 * https://www.ntnu.no/wiki/display/plab/9.+RFID+RC522
 * 
 * 
 * Reads RFID cards and compares them with two known cards.
 * 
 * Circuit:
 *   1 x RFID RC522
 * More about this reader can be found here:
 * https://www.ntnu.no/wiki/display/plab/9.+RFID+RC522
 */
// July/2014 Omer Siar Baysal
#include <SPI.h>
#include <MFRC522.h>

int successRead; // Variable integer to keep if we have Successful Read from Reader

byte readCard[4];           // Stores scanned ID read from RFID Module

/* We need to define MFRC522's pins and create instance
 * Pin layout should be as follows (on Arduino Uno):
 * MOSI: Pin 11 / ICSP-4
 * MISO: Pin 12 / ICSP-1
 * SCK : Pin 13 / ICSP-3
 * SS : Pin 10 (Configurable)
 * RST : Pin 9 (Configurable)
 * look MFRC522 Library for
 * pin configuration for other Arduinos.
 */

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance.

// My cards
byte myCard1[4] = {0xA1,0x17,0xE8,0x2B}; // Card #1
byte myCard2[4] = {0x7E,0x82,0x12,0x23}; // Card #2


///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {

  //Protocol Configuration
  Serial.begin(9600);	 // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware
}


///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop() {
  successRead = getID(); // sets successRead to 1 when we get read from reader otherwise 0
  if (successRead) ; //the program will not go further while you not get a successful read
  {
    if (areEqual(readCard,myCard1)) {
      Serial.println("Card 1."); };
    if (areEqual(readCard,myCard2)) {
      Serial.println("Card 2."); };
  }
}


int getID() {
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = 0;
  };
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return 0;
  }
  Serial.println("Scanned RFID code:");
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = mfrc522.uid.uidByte[i];
    Serial.print(readCard[i], HEX);
  }
  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}


boolean areEqual( byte a[], byte b[] ) {
  boolean match = true; // Assume they match at first
  for ( int k = 0; k < 4; k++ ) { // Loop 4 times
    if ( a[k] != b[k] ) // IF a != b then set match = false, one fails, all fail
      match = false;
  }
  return match;
}


