/* GBTec_RnD 2012.03.26 Ver.01 */
/* Light.cpp */

#include "Light.h"
#include "Arduino.h"

/*
Light::Light(int pinNum) {
	lightPin = pinNum;
	pinMode(lightPin, INPUT);
}
*/

void Light::begin(int pinNum) {
	lightPin = pinNum;
	pinMode(lightPin, INPUT);
}

int Light::read() {
	int val = analogRead(lightPin);
	//val = map(val, 0, 1024, 0, 100);

	return val;
}

Light gLight;