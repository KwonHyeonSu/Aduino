/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* UltraSonic.h */

#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include <inttypes.h>

class UltraSonic
{
public:
	//UltraSonic();
	//UltraSonic(int pinNum);

	void begin(int pinNum);
	int ReadDistanceMilimeter();
	int ReadDistanceCentimeter();
	int ReadDistanceInche();

private:
	int ultraPin;
	uint32_t getDuration();
};

extern UltraSonic gUltraSonic;
#endif //__ULTRASONIC_H__