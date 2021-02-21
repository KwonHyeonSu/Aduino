#ifndef __ALCOHOL_H__
#define __ALCOHOL_H__

#include <inttypes.h>

class Alcohol
{
public:
	void begin(uint8_t dataPin, uint8_t sclPin);
	int readValue();

private:
	int m_dataPin;
	int m_sclPin;
};

#endif //__ALCOHOL_H__