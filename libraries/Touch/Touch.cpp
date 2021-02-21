
#include "Touch.h"

Touch::Touch() {
	m_addr = 0;
}

void Touch::writeData(uint8_t addr, uint8_t data) {
	if(m_addr == 0) return ;

	Wire.beginTransmission(m_addr);
	Wire.write(addr);
	Wire.write(data);
	Wire.endTransmission();
}

void Touch::begin(uint8_t addr) {
	m_addr = addr;

	// I2C Initialize
	Wire.begin();

	// ANRG08 Initialize
	writeData(TG_CTRL1, 0x4D);
	writeData(TCH_EN, 0x7F);
	writeData(TCLK_CTRL, 0x06);
	writeData(TG_CTRL2, 0x4C);

	for(int i=0; i<TCH_NUM; i++) {
		writeData(TSENSE(i), TSENSE_VAL);
	}

	for(int i=0; i<3; i++) {
		writeData(TPWM(i), (byte)0);
	}

	writeData(TPORT_MODE, 0xFF);
	writeData(TG_CTRL1, 0x4C);
}

uint8_t Touch::getTouchKey() {
	uint8_t key = 0;

	Wire.beginTransmission(m_addr);
	Wire.write(TOUTPUT);
	Wire.endTransmission();

	Wire.requestFrom((int)m_addr, 1);
	if(Wire.available() > 0) {
		key = Wire.read();
	}

	return key;
}

void Touch::setLed(uint8_t led) {
	uint8_t aLed[3] = {0};

	if(led & 1) aLed[0] |= 0xf;
	if(led & 2) aLed[0] |= 0xf0;
	if(led & 4) aLed[1] |= 0xf;
	if(led & 8) aLed[1] |= 0xf0;
	if(led & 16) aLed[2] |= 0xf;

	for(int i=0; i<3; i++) {
		writeData(TPWM(i), aLed[i]);
    }
}

void Touch::setSensitivity(uint8_t sense) {
	m_sense = sense;

	for(int i=0; i<TCH_NUM; i++) {
		writeData(TSENSE(i), m_sense);
	}
}