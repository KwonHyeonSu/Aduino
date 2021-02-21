#include "Arduino.h"
#include "Line.h"

Line::Line() {
	m_pinNum = -1;
	m_th = 0;
}

void Line::begin(uint8_t pinNum) {
	m_pinNum = pinNum;
	pinMode(m_pinNum, INPUT);

	if(m_th == 0) setThreshold(DEFAULT_TH);
}

void Line::setThreshold(int th) {
	m_th = th;
}

int Line::isLine() {
	int val = 0;

	if(m_pinNum < 0) return -1;
	
	val = analogRead(m_pinNum);
	//Serial.print(val);

	if(val > m_th) return 1;
	else return 0;
}