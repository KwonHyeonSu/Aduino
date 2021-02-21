#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "Arduino.h"
#include <inttypes.h>
#include <Wire.h>

#define TCH_EN  0x01
#define TOUTPUT 0x2a
#define TCLK_CTRL  0x34
#define TG_CTRL1  0x36
#define TG_CTRL2  0x38
#define TSENSE(x)  (0x39+x)
#define TPWM(x)  (0x43+x)
#define TPORT_MODE  0x4f

#define TCH_NUM  5
#define TSENSE_VAL  0xa0

class Touch
{
public:
	Touch();
	void begin(uint8_t addr);
    uint8_t getTouchKey();
	void setLed(uint8_t led);
	void setSensitivity(uint8_t sense);

private:
	uint8_t m_addr;
	uint8_t m_sense;
	uint8_t m_led[4];

	void writeData(uint8_t addr, uint8_t data);
};

#endif