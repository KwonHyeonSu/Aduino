/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* PIR.h */

#ifndef __PIR_H__
#define __PIR_H__

#include <inttypes.h>

class PIR
{
public:
	/*
	PIR();
	PIR(uint8_t pinNum);
	PIR(int pinNum);

	void begin();
	*/
	void begin(uint8_t pinNum);
	int readValue();

private:
	uint8_t pirPin;

};

extern PIR gPIR;
#endif // __PIR_H__