/* GBTec_RnD 2012.03.26 Ver.01 */
/* DCMotor.cpp */

#include "DCMotor.h"
#include "Arduino.h"

/*
DCMotor::DCMotor(int pin1, int pin2) {
	dcPin1 = pin1;
	dcPin2 = pin2;
	dcSpeed = 255;

	pinMode(dcPin1, OUTPUT);
	pinMode(dcPin2, OUTPUT);
	digitalWrite(dcPin1, HIGH);
	digitalWrite(dcPin2, HIGH);
}
*/

void DCMotor::begin(int pin1, int pin2) {
	dcPin1 = pin1;
	dcPin2 = pin2;
	dcSpeed = 255;
	dcOn = false;

	pinMode(dcPin1, OUTPUT);
	pinMode(dcPin2, OUTPUT);
	analogWrite(dcPin1, 0);
	analogWrite(dcPin2, 0);
}

void DCMotor::setSpeed(int speed) {
	if(speed > 255) speed = 255;
	dcSpeed = speed;
}

void DCMotor::Start(uint8_t bBw) {
	if(bBw > 0) {
		analogWrite(dcPin1, dcSpeed);
		analogWrite(dcPin2, 0);
	} else {
		analogWrite(dcPin1, 0);
		analogWrite(dcPin2, dcSpeed);
	}

	dcOn = true;
}

void DCMotor::Stop() {
	analogWrite(dcPin1, 255);
	analogWrite(dcPin2, 255);
	dcOn = false;
}

bool DCMotor::getStateOn() {
	return dcOn;
}

DCMotor gDCMotor;