/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* Gas.h */

#ifndef __GAS_H__
#define __GAS_H__

#include <inttypes.h>

class Gas
{
public:
	//Gas(int pinNum);

	void begin(int pinNum);
	int read();

private:
	int gasPin;
};

extern Gas gGas;
#endif //__GAS_H__