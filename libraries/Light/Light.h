/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* Light.h */

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <inttypes.h>

class Light
{
public:
	//Light(int pinNum);

	void begin(int pinNum);
	int read();

private:
	int lightPin;
};

extern Light gLight;
#endif //__LIGHT_H__