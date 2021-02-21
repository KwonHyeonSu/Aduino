/* GBTec_RnD 2012.03.26 Ver.01 */
/* ThreeAsixGyro.cpp */

#include "ThreeAsixGyro.h"
#include "Wire.h"

void ThreeAsixGyro::begin(uint8_t addr) {
	gyroAddr = addr;
	Wire.begin();
	gyroInit();
}

int ThreeAsixGyro::readXValue() {
	return read(REG_GXH, REG_GXL);
}

int ThreeAsixGyro::readYValue() {
	return read(REG_GYH, REG_GYL);
}

int ThreeAsixGyro::readZValue(){
	return read(REG_GZH, REG_GZL);
}

int ThreeAsixGyro::read(uint8_t regh, uint8_t regl) {
	int value=0;

	value = read(regh);
	value = (value << 8) | read(regl);

	return value;
}

uint8_t ThreeAsixGyro::read(uint8_t reg) {
	uint8_t value=0;

	Wire.beginTransmission(gyroAddr);
	Wire.write(reg);
	Wire.endTransmission();

	Wire.requestFrom((int)gyroAddr, 1);
	if(Wire.available() > 0) {
		value = Wire.read();
	}

	return value;
}

void ThreeAsixGyro::gyroInit() {
	Wire.beginTransmission(gyroAddr);
	Wire.write(REG_PWRM);
	Wire.write(0x80);
	Wire.endTransmission();
	
	Wire.beginTransmission(gyroAddr);
	Wire.write(REG_SMPL);
	Wire.write(uint8_t(0));
	Wire.endTransmission();
	
	Wire.beginTransmission(gyroAddr);
	Wire.write(REG_DLPF);
	Wire.write(0x18);
	Wire.endTransmission();
}

ThreeAsixGyro gThreeAsixGyro;