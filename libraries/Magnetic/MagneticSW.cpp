#include "Arduino.h"
#include "MagneticSW.h"

MagneticSW::MagneticSW() {
	m_pinNum = -1;
	m_th = 0;
}

void MagneticSW::begin(uint8_t pinNum) {
	m_pinNum = pinNum;

	pinMode(m_pinNum, INPUT);
	if(m_th == 0) setThreshold(1000);
}

void MagneticSW::setThreshold(int th) {
	m_th = th;
}

int MagneticSW::isContact() {
	int val = 0;

	if(m_pinNum < 0) return -1;
	
	val = analogRead(m_pinNum);

	if(val < m_th) return 1;
	else return 0;
}