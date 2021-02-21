/* GBTec_RnD 2012.03.26 Ver.01 */
/* StepMotor.h */

#ifndef __STEPMOTOR_H__
#define __STEPMOTOR_H__

#include <inttypes.h>

#define FULL_STEP	0
#define	HALF_STEP	1
#define	QUART_STEP	2
#define	EIGHTH_STEP	3

class StepMotor
{
public:
	//StepMotor();

	void begin(uint8_t dir, uint8_t step, uint8_t ms1, uint8_t ms2, uint8_t sleep);
	void setSpeed(int spd);
	void setStep(uint8_t step);
	void on(uint8_t bOn=1);
	void off();
	void setDirection(uint8_t bBw=0);
	void setSleep(uint8_t bSleep=0);
	bool getStateOn();

private:
	uint8_t dirPin;
	uint8_t ms1Pin;
	uint8_t ms2Pin;
	uint8_t sleepPin;
	uint8_t stepPin;
	uint8_t step;
	int speed;
	bool bStepOn;
};

//void isrTimerOne();
extern StepMotor gStepMotor;

#endif //__STEPMOTOR_H__