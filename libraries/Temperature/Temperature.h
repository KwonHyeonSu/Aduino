/* GBTec_RnD 2012.03.26 Ver.01 */
/* Temperature.h */

#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

#include <inttypes.h>

#define REG_TEMP	0x00
#define REG_CONF	0x01
#define	REG_TLOW	0x02
#define	REG_THIGH	0x03

//#define TEMP_ADDR		72

class Temperature
{
public:
	//Temperature();

	void begin(uint8_t addr);
	int getTemperatureC();
	int getTemperatureF();

private:
	uint8_t tempAddr;
	float readTemperature();
};

extern Temperature gTemperature;
#endif //__TEMPERATURE_H__