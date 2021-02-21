/* GBTec_RnD 2012.03.26 Ver.01 */
/* LED.h */

#ifndef __LED_H__
#define __LED_H__

#include <inttypes.h>
#include "Wire.h"

#define REG_PORTA	0x02
//#define REG_PORTB	0x03
#define	REG_IOSETA	0x06
//#define REG_IOSETB	0x07

class LED
{
public:
	//LED();

	void begin(int addr);
	void setLed(uint8_t led);
	void On(uint8_t ledNum);
	void Off(uint8_t ledNum);
	void AllOn();
	void AllOff();
	void AllOnOff(uint8_t bOn=0);
	void ShiftRight();
	void ShiftLeft();

private:
	int ledAddr;
	int ledState;

};

extern LED gLED;
#endif //__LED_H__