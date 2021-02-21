/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* Humidity.h */

#ifndef __HUMIDITY_H__
#define __HUMIDITY_H__

#include <inttypes.h>

class Humidity
{
public:
	//Humidity(int pinNum);

	void begin(int pinNum);
	float read(float temp = 25.0);

private:
	int humPin;

	float getHumidity(float temp);
};

extern Humidity gHumidity;

#endif //__HUMIDITY_H__