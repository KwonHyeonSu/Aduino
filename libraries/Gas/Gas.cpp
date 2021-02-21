/* GBTec_RnD 2012.03.26 Ver.01 */
/* Gas.cpp */

#include "Gas.h"
#include "Arduino.h"

/*
Gas::Gas(int pinNum) {
	gasPin = pinNum;
	pinMode(gasPin, INPUT);
}
*/

void Gas::begin(int pinNum) {
	gasPin = pinNum;
	pinMode(gasPin, INPUT);
}

int Gas::read() {
	int val = analogRead(gasPin);
	//val = map(val, 0, 1024, 0, 100);

	return val;
}

Gas gGas;