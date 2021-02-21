/* GBTec_RnD 2012.03.26 Ver.01 */
/* RgbLed.cpp */

#include "RgbLed.h"
#include "Arduino.h"


void RgbLed::begin(uint8_t dPinNum, uint8_t cPinNum) {
	dataPin = dPinNum;
	clkPin = cPinNum;

	pinMode(dataPin, OUTPUT);
	pinMode(clkPin, OUTPUT);
}

void RgbLed::clockPulse() {
	digitalWrite(clkPin, LOW);
	delayMicroseconds(20);
	digitalWrite(clkPin, HIGH);
	delayMicroseconds(20);
}

void RgbLed::send32Zero() {
	int i;

	for(i=0; i<32; i++) {
		digitalWrite(dataPin, LOW);
		clockPulse();
	}
}

void RgbLed::sendData(uint32_t txData) {
	uint8_t i;

	for(i=0; i<32; i++) {
		if((txData & 0x80000000) != 0) {
			digitalWrite(dataPin, HIGH);
		} else {
			digitalWrite(dataPin, LOW);
		}

		txData = txData << 1;
		clockPulse();
	}
}

uint32_t RgbLed::setRgbData(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t value=0;

	value |= (uint32_t)0x03 << 30;
	value |= (uint32_t)takeAntiCode(b) << 28;
	value |= (uint32_t)takeAntiCode(g) << 26;
	value |= (uint32_t)takeAntiCode(r) << 24;

	value |= (uint32_t)b << 16;
	value |= (uint32_t)g << 8;
	value |= (uint32_t)r;

	return value;
}

uint8_t RgbLed::takeAntiCode(uint8_t data){
	uint8_t value=0;

	if((data & 0x80) == 0) {
		value |= 0x02;
	}
	if((data & 0x40) == 0) {
		value |= 0x01;
	}

	return value;
}

void RgbLed::OnRgb(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t rgbData=0;

	send32Zero();
	rgbData = setRgbData(r, g, b);
	sendData(rgbData);
	send32Zero();
}

void RgbLed::On() {
	OnRgb(0xFF, 0xFF, 0xFF);
}

void RgbLed::Off() {
	OnRgb(0, 0, 0);
}

RgbLed gRgbLed;