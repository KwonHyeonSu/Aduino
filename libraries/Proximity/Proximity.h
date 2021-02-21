#ifndef __PROXIMITY_H__
#define __PROXIMITY_H__

#include <inttypes.h>

class Proximity
{
public:
	void begin(uint8_t pinNum);
	int isDetected();

private:
	int m_pinNum;
};

#endif // __PROXIMITY_H__