/*
 * HexDump
 * 
 * Leser RFID kort og printer ut ID som hex.
 * 
 * Kretsen:
 *   1 x RFID RC522
 * Mer om RFID leseren kan du finne her:
 * https://www.ntnu.no/wiki/display/plab/9.+RFID+RC522
 * 
 * 
 * Reads RFID cards and prints the ID as hex.
 * 
 * Circuit:
 *   1 x RFID RC522
 * More about this reader can be found here:
 * https://www.ntnu.no/wiki/display/plab/9.+RFID+RC522
 */
// July/2014 Omer Siar Baysal
#include <SPI.h>
#include <MFRC522.h>

boolean successRead; // Variable integer to keep if we have Successful Read from Reader
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
  if (successRead)  //the program will not go further while you not get a successful read
  {
    printID(readCard);
  }
}

void printID(byte a[]) {
  Serial.print("{");
    for (int i = 0; i < 4; i++) {  // 
      Serial.print("0x");
      Serial.print(a[i], HEX);
      if (i < 3) 
          { Serial.print(",");};
    };
    Serial.println("} ");
}

boolean getID() {
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = 0;
  };
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
    return false;
  }
  for (int i = 0; i < 4; i++) {  // 
    readCard[i] = mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}


