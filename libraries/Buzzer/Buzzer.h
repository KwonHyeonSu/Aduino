/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* Buzzer.h */

#ifndef __BUZZER_H__
#define __BUZZER_H__

#include <inttypes.h>

#define TIMER_FREQ	500	// 3000 Hz


class Buzzer
{
public:
//		Buzzer();
//		Buzzer(int pinNum);

//		void begin();
	void begin(int pinNum);
	void On(uint8_t bOn);
	void On();
	void Off();
	void setFreq(int freq);
	uint8_t getIsOn();

private:
	uint8_t pinHigh;
	uint8_t bBuzOn;
	int buzFreq;
};

//void isrTimerOne();
extern Buzzer gBuzzer;

#endif // __BUZZER_H_