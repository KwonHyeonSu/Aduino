#ifndef __MAGNETICSW_H__
#define __MAGNETICSW_H__

#include <inttypes.h>

class MagneticSW
{
public:
	MagneticSW();
	void begin(uint8_t pinNum);
	void setThreshold(int th);
	int isContact();

private:
	int m_pinNum;
	int m_th;
};

#endif //__MAGNETICSW_H__