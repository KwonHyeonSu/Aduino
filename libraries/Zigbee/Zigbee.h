#ifndef __ZIGBEE_H__
#define __ZIGBEE_H__

#include "Arduino.h"

class Zigbee
{
	public:
		void begin();
		void enterCommandMode();
		void sendCommand(char* chData, boolean bCR);
};

#endif