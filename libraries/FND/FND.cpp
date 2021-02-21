/* GBTec_RnD 2012.03.26 Ver.01 */
/* FND.cpp */

#include "FND.h"
#include "Arduino.h"


uint8_t fndData[16] = {0x03, 0x9f, 0x25, 0x0d, 0x99, 0x49, 0x41,
                0x1f, 0x01, 0x09, 0x11, 0xc1, 0xe5, 0x85, 0x61, 0x71};

/*
FND::FND() {
}
*/

void FND::begin(int addr1, int addr2) {
	fndAddr1 = addr1;
	fndAddr2 = addr2;

	Wire.begin();
	
	Wire.beginTransmission(fndAddr1);
	Wire.write(REG_IOSETA);
	Wire.write(byte(0));
	Wire.endTransmission();
	
	Wire.beginTransmission(fndAddr1);
	Wire.write(REG_IOSETB);
	Wire.write(byte(0));
	Wire.endTransmission();

	Wire.beginTransmission(fndAddr2);
	Wire.write(REG_IOSETA);
	Wire.write(byte(0));
	Wire.endTransmission();
	
	Wire.beginTransmission(fndAddr2);
	Wire.write(REG_IOSETB);
	Wire.write(byte(0));
	Wire.endTransmission();

	setAllNum(0,0,0,0);
}

void FND::setFndVal(uint8_t addr, uint8_t reg, uint8_t val) {
	Wire.beginTransmission(addr);
	Wire.write(reg);
	Wire.write(val);
	Wire.endTransmission();
}

void FND::setNum(uint8_t fndNum, uint8_t fndVal, uint8_t bDot) {
	uint8_t addr=0;
	uint8_t reg=0;
	uint8_t val=fndData[fndVal];

	if(fndVal > 0xf) return;
	if(bDot > 0) val--;

	switch(fndNum) {
	case 1:
		addr = fndAddr1;  reg = REG_PORTB;  break;
	case 2:
		addr = fndAddr1;  reg = REG_PORTA;  break;
	case 3:
		addr = fndAddr2;  reg = REG_PORTB;  break;
	case 4:
		addr = fndAddr2;  reg = REG_PORTA;  break;
	}

	setFndVal(addr, reg, val);
}

void FND::setAllNum(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4) {
	setNum(1, num1);
	setNum(2, num2);
	setNum(3, num3);
	setNum(4, num4);
}

void FND::setAllNum(int num) {
	int temp = num;

	if(temp > 9999) temp -= 10000;

	setNum(1, temp/1000);
	temp = temp % 1000;

	setNum(2, temp/100);
	temp = temp % 100;

	setNum(3, temp/10);
	temp = temp % 10;

	setNum(4, temp);
}

FND gFND;