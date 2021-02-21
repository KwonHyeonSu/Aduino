/* GBTec_RnD 2012.03.26 Ver.01 */
/* Sound.cpp */

#include "Sound.h"
#include "Arduino.h"

/*
Sound::Sound(int pinNum) {
	soundPin = pinNum;
	pinMode(soundPin, INPUT);
}
*/

void Sound::begin(int pinNum) {
	soundPin = pinNum;
	pinMode(soundPin, INPUT);
}

int Sound::read() {
	int val = analogRead(soundPin);
	//val = map(val, 0, 1024, 0, 100);

	return val;
}

Sound gSound;