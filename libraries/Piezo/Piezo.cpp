/* GBTec_RnD 2012.03.26 Ver.01 */
/* Piezo.cpp */

#include "Piezo.h"
#include "Arduino.h"

int piezoPin;

/*
Piezo::Piezo(int pinNum) {
	piezoPin = pinNum;
	pinMode(piezoPin, INPUT);
	timerFreq = 200;
}
*/

void Piezo::begin(int pinNum) {
	piezoPin = pinNum;
	pinMode(piezoPin, INPUT);
	timerFreq = 2000;
}

int Piezo::read() {
	int val = analogRead(piezoPin);
	//val = map(val, 0, 1024, 0, 100);

	return val;
}

void Piezo::setFreq(int freq) {
	timerFreq = freq;
}

void Piezo::setMode(int mode) {
	if(mode == OUTPUT) {
		tone(piezoPin, timerFreq);
	} else {
		noTone(piezoPin);
	}
	pinMode(piezoPin, mode);
}


Piezo gPiezo;