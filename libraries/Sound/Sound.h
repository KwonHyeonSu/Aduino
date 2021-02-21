/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* Sound.h */

#ifndef __SOUND_H__
#define __SOUND_H__

#include <inttypes.h>

class Sound
{
public:
	//Sound(int pinNum);

	void begin(int pinNum);
	int read();

private:
	int soundPin;
};

extern Sound gSound;
#endif //__SOUND_H__