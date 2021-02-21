/* GBTec_RnD 2012.03.26 Ver.01 */
/* Switch.h */

#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <inttypes.h>

class Switch
{
public:
	// Switch();

	void begin();
	uint8_t getPushKey();
	uint8_t changePushKey(uint8_t key);
	uint8_t waitGetPushKey();

private:
	uint8_t bPushKey;
	uint8_t pushKeyNum;
};

extern Switch gSwitch;
#endif //__SWITCH_H__