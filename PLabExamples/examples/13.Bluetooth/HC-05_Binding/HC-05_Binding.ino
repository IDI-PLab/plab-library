/*
Binding

This is code to pair HC-05 Modules as Master to another BT module.

It requires the following connection of the pins:

Arduino  HC-05
GND    ---  GND
Pin 2  ---  RXD
Pin 3  ---  TXD
Pin 4  ---  KEY
Pin 5  ---  VCC

Yes, the power (VCC) is connected to a pin and not to VCC on the Arduino.
This is because the module needs to be turned on and off from software 
during the pairing process.

The code runs scan only once. If your module does not appear in the list,
restart the Arduino by pressing the reset button (the red button by the USB)
to initiate a new scan.

After pairing, the module will work as a serial port connection running on 9600 baud.


https://www.ntnu.no/wiki/pages/viewpage.action?pageId=78024129

Dag Svanæs 2015

 */

#include <SoftwareSerial.h>
#include <PLabBTSerial.h>

#define rxPin 2
#define txPin 3
#define keyPin 4
#define pwrPin 5

PLabBTSerial btSerial(txPin, rxPin);
// SoftwareSerial btSerial =  SoftwareSerial(txPin, rxPin);

void setup()
{
    Serial.begin(9600);
    Serial.println("PLab Bluetooth unit pairing.");
    Serial.println("Please wait (5 secs.).");
    pinMode(pwrPin, OUTPUT);
    pinMode(keyPin, OUTPUT);
    digitalWrite(pwrPin,LOW);
    digitalWrite(keyPin,LOW);
    myDelay(5000);
    digitalWrite(keyPin,HIGH);
    myDelay(500);
    digitalWrite(pwrPin,HIGH);   
  
    btSerial.begin(38400);
    run();
}

void echo() {
      Serial.print(":");
      while (btSerial.available()) {
       char c = btSerial.read();
       Serial.write(c);
      };
      Serial.println();
};

void readIfDataBT() {
  int availableCount = btSerial.available();
  if (availableCount > 0) {
     // Create a char array (c-string) to hold the incomming data.
    char text[availableCount];
    // Read the incomming data. bufferSize refers to the buffer you are giving as an argument
    btSerial.read(text, availableCount);
    // Print the incomming data to console
 //   Serial.print("Received data: ");
 //   Serial.println(text);
  }
}

String readBT() {
  while(!btSerial.available()) {};
  int availableCount = btSerial.available();
  if (availableCount > 0) {
     // Create a char array (c-string) to hold the incomming data.
    char text[availableCount];
    for (int i=0; i < availableCount;i++) {text[i]=' ';};
    // Read the incomming data. bufferSize refers to the buffer you are giving as an argument
    btSerial.read(text, availableCount);
    // Print the incomming data to console
   // Serial.print("Received data: ");
   // Serial.println(text);
    return String(text);
  };
  return "";
}

String stringArr[10];
int noOfBTSlavesFound = 0;

void myDelay(int milli) {
  for (int i = 0; i < milli; i+=500) {
    Serial.print(".");
    delay(500);
  };
  Serial.println();
}



void run() {
    while (!Serial.available()) {
      Serial.println("Type <enter> key to start pairing of BT modules.");
      delay(2000);
    };
    while (Serial.available()) {  // wait for user to type...
      char c = Serial.read();
    }; 

    btSerial.write("AT+ORGL\r\n"); delay(100); echo();
    btSerial.write("AT+RMAAD\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+PSWD=1234\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+ROLE=1\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+CMODE=0\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+INIT\r\n"); delay(100); readIfDataBT();  
    btSerial.write("at+inqm=1,10,4\r\n"); delay(100); readIfDataBT();
    btSerial.write("AT+INQ\r\n"); delay(500); readIfDataBT();
    Serial.println("Searching for BT units (20 secs.):"); myDelay(20000);
    noOfBTSlavesFound = 0;
    String myString = "";
 //   while (myString.indexOf("OK") < 0) {
    while (btSerial.available()) {
         myString = readBT();
         stringArr[noOfBTSlavesFound] = myString;
         noOfBTSlavesFound++;
      // Print the incomming data to console
 //        Serial.print("BT Unit code: ");
 //        Serial.println(myString);
    };
  //  noOfBTSlavesFound--;
    Serial.print("No of BT modules found: "); Serial.println(noOfBTSlavesFound);
    for (int i=0; i < noOfBTSlavesFound; i++) {
      int c1 = stringArr[i].indexOf(':');
      int c2 = stringArr[i].indexOf(':', c1+1);
      int c3 = stringArr[i].indexOf(':', c2+1);
      int c4 = stringArr[i].indexOf(',', c3+1);  
      String s1 = stringArr[i].substring(c1+1,c2); 
      String s2 = stringArr[i].substring(c2+1,c3);
      String s3 = stringArr[i].substring(c3+1,c4);       
 //     Serial.println(stringArr[i]);
 //     Serial.println(s1+","+s2+","+s3);
      String s = "AT+RNAME?"+s1+","+s2+","+s3;
      char chars[s.length()];
      s.toCharArray(chars,s.length()+1);
//      Serial.println(chars);
      btSerial.write(chars); btSerial.write("\r\n");
      Serial.println("Getting name for BT unit (10 secs.):"); myDelay(10000); 
      Serial.print("BT Unit ");
      Serial.print(i+1); Serial.print(": ");
      while (btSerial.available()) {
        int availableCount = btSerial.available();
        if (availableCount > 0) {
         // Create a char array (c-string) to hold the incomming data.
           char text[availableCount];
           // Read the incomming data. bufferSize refers to the buffer you are giving as an argument
           btSerial.read(text, availableCount);
           String myString = String(text);
        // Print the incomming data to console           
           Serial.println(myString);     
          }
       };      
    };
    if (noOfBTSlavesFound > 0) {
      Serial.print("Type number of BT unit to pair (1 - ");
      Serial.print(noOfBTSlavesFound); Serial.println("):");
      while (!Serial.available()) { } // wait for user to type... 
      char c = Serial.read(); delay(500);
      while (Serial.available()) {  // wait for user to type... 
         char temp = Serial.read();
      };
      int id = c - '0';
      Serial.println(id);
      int i = id - 1;
      int c1 = stringArr[i].indexOf(':');
      int c2 = stringArr[i].indexOf(':', c1+1);
      int c3 = stringArr[i].indexOf(':', c2+1);
      int c4 = stringArr[i].indexOf(',', c3+1);  
      String s1 = stringArr[i].substring(c1+1,c2); 
      String s2 = stringArr[i].substring(c2+1,c3);
      String s3 = stringArr[i].substring(c3+1,c4);       
 //     Serial.println(stringArr[i]);
 //     Serial.println(s1+","+s2+","+s3);
      btSerial.write("AT+CMODE=0\r\n"); delay(100); readIfDataBT();
      String s = "AT+BIND="+s1+","+s2+","+s3;
      char chars[s.length()];
      s.toCharArray(chars,s.length()+1);
      Serial.println(chars);
      btSerial.write(chars); btSerial.write("\r\n");
      btSerial.write("AT+BIND?\r\n"); delay(100); echo();
      btSerial.write("AT+UART=9600,1,0\r\n"); delay(100); echo();
      btSerial.write("AT+UART?\r\n"); delay(100); echo();
      Serial.print("BT Unit "); 
      Serial.print(id); 
      Serial.println(" paired successfully. Please wait for it to connect."); 
      Serial.println("The red LED on the slave BT unit should stop blinking"); 
      digitalWrite(pwrPin,LOW);
      digitalWrite(keyPin,LOW);
      delay(1000);
      digitalWrite(pwrPin,HIGH);
      delay(1000);
      while (Serial.available()) {
        char c = Serial.read();
      }; // wait for user to type... 
      Serial.println("Type <enter> key to send test string STEPF to unit.");
      while (!Serial.available()) { } // wait for user to type... 
      c = Serial.read();
      btSerial.begin(9600);
      Serial.println("STEPF");     
      btSerial.write("STEPF\r\n");  
      
    };
    
}

void loop()
{}

