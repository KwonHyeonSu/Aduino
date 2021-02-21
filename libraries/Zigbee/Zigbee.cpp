#include "Zigbee.h"


void Zigbee::begin() {
	Serial2.begin(9600);
}

void Zigbee::enterCommandMode() {
	boolean bAck=false;
	char rxData[3] = {0};
	byte rxCnt=0;

	Serial2.print("+++");
	while(!bAck) {
		if(Serial2.available() > 0) {
			rxData[rxCnt++] = Serial2.read();
			if(rxCnt == 2) {
				String strData = String(rxData);
				if(strData == "OK") {
					bAck = true;
				} else {
					bAck = true;
				}
			}
		}
	}
	delay(100);
}

void Zigbee::sendCommand(char* chData, boolean bCR) {
  boolean bAck=false;
  char rxData[16]={0};
  byte rxCnt=0;
  
  Serial2.print(chData);
  if(bCR) Serial2.println();
  
  while(!bAck) {
    if(Serial2.available() > 0) {
      char ch = Serial2.read();
      rxData[rxCnt++] = ch;
      if(rxCnt==2) {
        String strData = String(rxData);
        if(strData == "OK") {
          bAck = true;
          rxCnt=0;
        } else {
          bAck = true;
          rxCnt = 0;
        }
      }
    }
  }
  Serial.println();
}