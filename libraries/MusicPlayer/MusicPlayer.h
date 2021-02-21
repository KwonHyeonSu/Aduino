#ifndef __MUSICPLAYER_H__
#define __MUSICPLAYER_H__

#include "Arduino.h" 
#include "HardwareSerial.h"

#define CMD_START		0x7E
#define CMD_END			0x7E
#define CMD_PLAY		0xA0
#define CMD_PAUSE		0xA3
#define CMD_STOP		0xA4
#define CMD_NEXT		0xA5
#define CMD_PREV		0xA6
#define CMD_VOLUME		0xA7
#define CMD_MODE		0xA9
#define CMD_GETTRACK	0xC6

#define MODE_SINGLE		0x00
#define MODE_REPEAT		0x01
#define MODE_ALL		0x02
#define MODE_RANDOM		0x03

#define MPSERIAL	Serial1

#define BUFFER_LEN	8

class MusicPlayer
{
	
private:
	byte cmdBuf[BUFFER_LEN];

	char sendCommand(byte len);
	byte setCommand(byte cmd, int data=0);

public:
	
	void begin(uint16_t baud);

	void Play(int track);
	void Pause();
	void Stop();
	void Next();
	void Previous();
	void SetVolume(byte vol);
	void SetPlayMode(byte mode);
	
	int GetTrackNumber();
};

#endif // __MUSICPLAYER_H__