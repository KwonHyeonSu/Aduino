/* GBTec_RnD 2012.03.26 Ver.01 */
/* StepMotor.cpp */

#include "StepMotor.h"
#include "Arduino.h"
//#include "TimerOne.h"

#define DEFAULT_SPEED	500


void StepMotor::begin(uint8_t dir, uint8_t step, uint8_t ms1, uint8_t ms2, uint8_t sleep) {
	dirPin = dir;
	stepPin = step;
	ms1Pin = ms1;
	ms2Pin = ms2;
	sleepPin = sleep;

	pinMode(dirPin, OUTPUT);
	pinMode(stepPin, OUTPUT);
	pinMode(ms1Pin, OUTPUT);
	pinMode(ms2Pin, OUTPUT);
	pinMode(sleepPin, OUTPUT);

	setDirection();
	setSpeed(DEFAULT_SPEED);
	setStep(FULL_STEP);
	setSleep(1);

	bStepOn = false;

	//Timer1.attachInterrupt(isrTimerOne);
}

void StepMotor::setSpeed(int spd) {
	//if(spd > 255) spd = 255;

	speed = spd;
}

void StepMotor::setStep(uint8_t st) {
	uint8_t ms1, ms2;

	if(st > 3) return;
	step = st;

	switch(step) {
		case FULL_STEP:
			ms1 = LOW;	ms2 = LOW;	break;
		case HALF_STEP:
			ms1 = HIGH;	ms2 = LOW;	break;
		case QUART_STEP:
			ms1 = LOW;	ms2 = HIGH;	break;
		case EIGHTH_STEP:
			ms1 = HIGH;	ms2 = HIGH;	break;
	}

	digitalWrite(ms1Pin, ms1);
	digitalWrite(ms2Pin, ms2);
}

void StepMotor::on(uint8_t bOn) {
	if(bOn > 0) {
		setSleep(0);
		//Timer1.initialize(speed);
		//analogWrite(stepPin, speed);
		tone(stepPin, speed);
		bStepOn = true;
	} else {
		off();
	}
}

void StepMotor::off() {
	//Timer1.stop();
	//analogWrite(stepPin, 0);
	setSleep(1);
	noTone(stepPin);
	bStepOn = false;
}

void StepMotor::setDirection(uint8_t bBw) {
	digitalWrite(dirPin, bBw);
}

void StepMotor::setSleep(uint8_t bSleep) {
	if(bSleep > 0) {
		digitalWrite(sleepPin, LOW);
	} else {
		digitalWrite(sleepPin, HIGH);
	}
}

bool StepMotor::getStateOn() {
	return bStepOn;
}

/*
void isrTimerOne() {
	if(digitalRead(stepPin) == 0) {
		digitalWrite(stepPin, 1);
	} else {
		digitalWrite(stepPin, 0);
	}
}
*/

StepMotor gStepMotor;