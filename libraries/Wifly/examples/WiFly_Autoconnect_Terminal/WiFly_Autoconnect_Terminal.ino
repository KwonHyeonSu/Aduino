/*
 * Web Server
 *
 * (Based on Ethernet's WebServer Example)
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include "WiFly.h"
#include "Credentials.h"


void setup() {
  Serial.begin(9600);
  Serial.println("\n\rWait....");
  WiFly.begin();

  if (!WiFly.join(ssid, passphrase)) {
    Serial.println("Association failed.");
    while (1) {
      // Hang on failure.
    }
  }
  SpiSerial.println("exit");
  delay(100);
  Serial.println("\n\r\n\rWiFly Shield Terminal Routine :  \"$$$\"");
}

void loop() {
  int readVal;
    
  while(SpiSerial.available() > 0) {
    Serial.print((char)SpiSerial.read());
  }
  
  if(Serial.available()) { // Outgoing data
    readVal = Serial.read();
    SpiSerial.print((char)readVal);
  }
}
