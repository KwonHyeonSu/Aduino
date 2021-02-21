/* GBTec_RnD 2012.03.26 Ver.01 */
/* FND.h */

#ifndef __FND_H__
#define __FND_H__

#include <inttypes.h>
#include "Wire.h"

#define REG_PORTA	0x02
#define REG_PORTB	0x03
#define	REG_IOSETA	0x06
#define REG_IOSETB	0x07

class FND
{
public:
	//FND();

	void begin(int addr1, int addr2);
	void setFndVal(uint8_t addr, uint8_t reg, uint8_t val);
	void setNum(uint8_t fndNum, uint8_t fndVal, uint8_t bDot=0);
	void setAllNum(uint8_t num1, uint8_t num2, uint8_t num3, uint8_t num4);
	void setAllNum(int num);

private:
	uint8_t fndAddr1;
	uint8_t fndAddr2;
};

extern FND gFND;
#endif //__LED_H__ 