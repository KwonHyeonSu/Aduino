/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* PIR.cpp */

#include "PIR.h"
#include "Arduino.h"

/*
PIR::PIR() {
	pirPin = 0;
}

PIR::PIR(uint8_t pinNum) {
	pirPin = pinNum;
}

PIR::PIR(int pinNum) {
	pirPin = (uint8_t)pinNum;
}

void PIR::begin() {
	pinMode(pirPin, INPUT);
}
*/

void PIR::begin(uint8_t pinNum) {
	pirPin = pinNum;
	pinMode(pirPin, INPUT);
}

int PIR::readValue() {
	return digitalRead(pirPin);
}

PIR gPIR;