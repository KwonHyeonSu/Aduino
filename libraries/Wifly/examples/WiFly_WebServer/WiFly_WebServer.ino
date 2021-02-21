/*
 * Web Server
 *
 * (Based on Ethernet's WebServer Example)
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include "WiFly.h"
#include "Credentials.h"


Server server(80);

void setup() {
  WiFly.begin();

  if (!WiFly.join(ssid, passphrase)) {
    while (1) {
      // Hang on failure.
    }
  }

  Serial.begin(9600);
  Serial.print("IP: ");
  Serial.println(WiFly.ip());
  
  server.begin();
}

void loop() {
  int readVal;
  Client client = server.available();
  
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    
   // Serial.println("Client Connect OK!! - yulee");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.println("Client Connect OK!! - yulee");
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        //if (c == '\n' && current_line_is_blank) {
          if(true) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          // output the value of each analog input pin
          for (int i = 0; i < 6; i++) {
            client.print("analog input ");
            client.print(i);
            client.print(" is ");
            client.print(analogRead(i));
            client.println("<br />");
          }
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
  }
  
  while(SpiSerial.available() > 0) {
    Serial.print((char)SpiSerial.read());
  }
  
  if(Serial.available()) { // Outgoing data
    readVal = Serial.read();
    SpiSerial.print((char)readVal);
  }
}
