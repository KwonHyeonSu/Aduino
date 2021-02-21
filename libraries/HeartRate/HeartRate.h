#ifndef __HEARTRATE_H__
#define __HEARTRATE_H__

#include <inttypes.h>

class HeartRate
{
public:
	void begin(uint8_t pinNum);
	int getHeartRateforMin();
	void startMeasure();
	int getValue();
private:
	int m_pinNum;
	int m_intNum;
	unsigned long startMillis;
};


#endif //__HEARTRATE_H__