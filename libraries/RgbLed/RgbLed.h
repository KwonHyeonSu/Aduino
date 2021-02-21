/* GBTec_RnD 2012.03.26 Ver.01 */
/* RgbLed.h */

#ifndef __RGBLED_H__
#define __RGBLED_H__

#include <inttypes.h>

class RgbLed
{
public:
	// RgbLed();

	void begin(uint8_t dPinNum, uint8_t cPinNum);
	void OnRgb(uint8_t r, uint8_t g, uint8_t b);
	void On();
	void Off();

private:
	uint8_t dataPin;
	uint8_t clkPin;
	
	void clockPulse();
	void send32Zero();
	void sendData(uint32_t txData);
	uint32_t setRgbData(uint8_t r, uint8_t g, uint8_t b);
	uint8_t takeAntiCode(uint8_t data);
};

extern RgbLed gRgbLed;
#endif //__RGBLED_H__