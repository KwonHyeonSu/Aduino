#include "Buzzer.h"
#include "Arduino.h"
//#include "TimerOne.h"

int buzPin=0;

/*
Buzzer::Buzzer() {
	buzPin = 0;
	buzFreq = TIMER_FREQ;
	bBuzOn = 0;
}

Buzzer::Buzzer(int pinNum) {
	buzPin = pinNum;
	buzFreq = TIMER_FREQ;
	bBuzOn = 0;
}

void Buzzer::begin() {
	if(buzPin == 0) return;

	pinMode(buzPin, OUTPUT);
	Timer1.attachInterrupt(isrTimerOne);
}
*/

void Buzzer::begin(int pinNum) {
	buzPin = pinNum;
	buzFreq = TIMER_FREQ;
	
	pinMode(buzPin, OUTPUT);
	//Timer1.attachInterrupt(isrTimerOne);

	bBuzOn = 0;
}

void Buzzer::On(uint8_t bOn) {
	if(bOn > 0) {
		On();
	} else {
		Off();
	}
}

void Buzzer::On() {
	bBuzOn = 1;
	//Timer1.initialize(buzFreq);
	tone(buzPin, buzFreq);
}

void Buzzer::Off() {
	bBuzOn = 0;
	//Timer1.stop();
	noTone(buzPin);
}

void Buzzer::setFreq(int freq) {
	//if(bBuzOn > 0) Timer1.stop();
	if(bBuzOn > 0) noTone(buzPin);
	buzFreq = freq;
}

uint8_t Buzzer::getIsOn() {
	return bBuzOn;
}

/*
void isrTimerOne() {
	if(digitalRead(buzPin) == 0) {
		digitalWrite(buzPin, 1);
	} else {
		digitalWrite(buzPin, 0);
	}
}
*/
Buzzer gBuzzer;