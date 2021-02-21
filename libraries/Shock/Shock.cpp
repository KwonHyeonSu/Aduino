#include "Arduino.h"
#include "Shock.h"

void Shock::begin(uint8_t littlePin, uint8_t bigPin) {
	m_littlePin = littlePin;
	m_bigPin = bigPin;

	pinMode(m_littlePin, INPUT);
	pinMode(m_bigPin, INPUT);
}

int Shock::readLittleShock() {
	//return digitalRead(m_littlePin);
	if(digitalRead(m_littlePin) == LOW) return 1;
	else return 0;
}

int Shock::readBigShock() {
	//return digitalRead(m_bigPin);
	if(digitalRead(m_bigPin) == LOW) return 1;
	else return 0;
}