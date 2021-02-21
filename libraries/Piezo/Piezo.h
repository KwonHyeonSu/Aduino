/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* Piezo.h */

#ifndef __PIEZO_H__
#define __PIEZO_H__

#include <inttypes.h>

class Piezo
{
public:
	//Piezo(int pinNum);

	void begin(int pinNum);
	int read();
	void setFreq(int freq);
	void setMode(int mode);

private:
	int timerFreq;
};

extern Piezo gPiezo;
#endif //__PIEZO_H__