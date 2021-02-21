/* GBTec_RnD 2012.03.26 */
/* Temperature.cpp */

#include "Temperature.h"
#include "Wire.h"


void Temperature::begin(uint8_t addr) {
	tempAddr = addr;
	Wire.begin();
}

int Temperature::getTemperatureC() {
	return (int)readTemperature();
}

int Temperature::getTemperatureF() {
	float fTemp = readTemperature();
	fTemp = (fTemp * 9 / 5) + 32;

	return (int)fTemp;
}

float Temperature::readTemperature() {
	uint8_t hValue=0;
	uint8_t lValue=0;
	int iValue=0;
	float fValue=0;

	Wire.beginTransmission(tempAddr);
	Wire.write(uint8_t(REG_TEMP));
	Wire.endTransmission();

	Wire.requestFrom((int)tempAddr, 2);
	hValue = Wire.read();
	lValue = Wire.read();

	iValue = hValue;
	iValue = (iValue << 8) | lValue;
	iValue = iValue >> 4;

	fValue = (float)iValue * 0.0625;

	return fValue;
}

Temperature gTemperature;