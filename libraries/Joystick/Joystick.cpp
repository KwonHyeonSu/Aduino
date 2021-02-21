#include "Arduino.h"
#include "Joystick.h"

Joystick::Joystick() {
	m_xPin = -1;
	m_yPin = -1;
	m_sPin = -1;
}

void Joystick::begin(uint8_t xpin, uint8_t ypin, uint8_t spin) {
	m_xPin = xpin;
	m_yPin = ypin;
	m_sPin = spin;

	pinMode(m_xPin, INPUT);
	pinMode(m_yPin, INPUT);
	pinMode(m_sPin, INPUT);

	setCenterPos();
}

void Joystick::setCenterPos() {
	if(m_xPin < 0 || m_yPin < 0) return ;

	m_xCenterPos = analogRead(m_xPin);
	m_yCenterPos = analogRead(m_yPin);
}

int Joystick::getXPos() {
	int pos = 0;
	int retVal = 0;

	if(m_xPin < 0) return 0;

	pos = analogRead(m_xPin);
	retVal = pos - m_xCenterPos;
	retVal = retVal / JOY_TH;

	return retVal;
}

int Joystick::getYPos() {
	int pos = 0;
	int retVal = 0;

	if(m_yPin < 0) return 0;

	pos = analogRead(m_yPin);
	retVal = pos - m_yCenterPos;
	retVal = retVal / JOY_TH;

	return retVal;
}

int Joystick::isPushKey() {
	if(m_sPin < 0) return 0;
	if(digitalRead(m_sPin) == HIGH) return 0;
	else return 1;
}