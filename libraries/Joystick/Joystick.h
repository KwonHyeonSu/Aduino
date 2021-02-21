#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <inttypes.h>

#define JOY_TH	100

class Joystick
{
public:
	Joystick();
	void begin(uint8_t xpin, uint8_t ypin, uint8_t spin);
	void setCenterPos();
	int getXPos();
	int getYPos();
	int isPushKey();

private:
	int m_xPin;
	int m_yPin;
	int m_sPin;

	int m_xCenterPos;
	int m_yCenterPos;
};

#endif