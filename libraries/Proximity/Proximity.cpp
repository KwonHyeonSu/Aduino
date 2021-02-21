#include "Arduino.h"
#include "Proximity.h"

void Proximity::begin(uint8_t pinNum) {
	m_pinNum = pinNum;

	pinMode(m_pinNum, INPUT);
}

int Proximity::isDetected() {
	if(digitalRead(m_pinNum) == LOW) return 1;
	else return 0;
}