/* GBTec_RnD 2012.03.26 Ver.01 */
/* LED.cpp */

#include "LED.h"
#include "Arduino.h"

/*
LED::LED() {
	ledState = 0;
}
*/

void LED::begin(int addr) {
	ledAddr = addr;

	Wire.begin();
	Wire.beginTransmission(ledAddr);
	Wire.write(REG_IOSETA);
	Wire.write(byte(0));
	Wire.endTransmission();

	ledState = 0;
	setLed(ledState);
}

void LED::setLed(uint8_t led) {
	Wire.beginTransmission(ledAddr);
	Wire.write(REG_PORTA);
	Wire.write(~led);
	Wire.endTransmission();
}

void LED::On(uint8_t ledNum) {
	ledState |= (1 << (ledNum-1));
	setLed(ledState);
}

void LED::Off(uint8_t ledNum) {
	ledState &= ~(1<< (ledNum-1));
	setLed(ledState);
}

void LED::AllOn() {
	ledState = 0xFF;
	setLed(ledState);
}

void LED::AllOff() {
	ledState = 0;
	setLed(ledState);
}

void LED::AllOnOff(uint8_t bOn) {
	if(bOn > 0) AllOn();
	else		AllOff();
}

void LED::ShiftRight() {
	int temp = 0;
	
	if(ledState & 1) temp=0x80;
	ledState = (ledState >> 1) | temp;
	setLed(ledState);
}

void LED::ShiftLeft() {
	int temp = 0;

	if(ledState & 0x80) temp = 1;
	ledState = (ledState << 1) | temp;
	setLed(ledState);
}

LED gLED;