#include "Arduino.h"
#include "Alcohol.h"

void Alcohol::begin(uint8_t dataPin, uint8_t sclPin) {
	m_dataPin = dataPin;
	m_sclPin = sclPin;

	pinMode(m_dataPin, INPUT);
	pinMode(m_sclPin, OUTPUT);
	digitalWrite(m_sclPin, LOW);
}

int Alcohol::readValue() {
	return 1024 - analogRead(m_dataPin);
}