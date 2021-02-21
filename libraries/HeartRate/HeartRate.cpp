#include "Arduino.h"
#include "HeartRate.h"


int nHeartRate;
void heartRateISR();

void HeartRate::begin(uint8_t pinNum) {
	m_pinNum = pinNum;

	switch(pinNum) {
		case 2: m_intNum=0; break;
		case 3: m_intNum=1; break;
		case 18: m_intNum=5; break;
		case 19: m_intNum=4; break;
		case 20: m_intNum=3; break;
		case 21: m_intNum=2; break;
		default: return;
	}

	pinMode(m_pinNum, INPUT);
	attachInterrupt(m_intNum, heartRateISR, RISING);
	interrupts();
}

void HeartRate::startMeasure()
{
	startMillis = millis();
	nHeartRate = 0;
}

int HeartRate::getValue()
{	
	return nHeartRate;
}

int HeartRate::getHeartRateforMin() {
	unsigned long preMillis = millis();

	nHeartRate = 0;
	while(millis() - preMillis < 60000) ;
	return nHeartRate;
}

void heartRateISR() {
	nHeartRate++;
}