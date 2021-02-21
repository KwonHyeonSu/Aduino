/* GBTec_RnD 2012.03.26 Ver.01 */
/* DCMotor.h */

#ifndef __DCMOTOR_H__
#define __DCMOTOR_H__

#include <inttypes.h>


class DCMotor
{
public:
	//DCMotor(int pin1, int pin2);
	void begin(int pin1, int pin2);
	void setSpeed(int speed);
	void Start(uint8_t bBw=0);
	void Stop();
	bool getStateOn();

private:
	int dcPin1;
	int dcPin2;
	uint8_t dcSpeed;
	bool dcOn;
};

extern DCMotor gDCMotor;

#endif //__DCMOTOR_H__