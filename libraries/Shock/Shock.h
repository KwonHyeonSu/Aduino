#ifndef __SHOCK_H__
#define __SHOCK_H__

#include "inttypes.h"

class Shock
{
public:
	void begin(uint8_t littlePin, uint8_t bigPin);
	int readLittleShock();
	int readBigShock();

private:
	uint8_t m_littlePin;
	uint8_t m_bigPin;
};

#endif // __SHOCK_H__