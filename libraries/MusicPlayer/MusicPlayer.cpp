#include "MusicPlayer.h"


void MusicPlayer::begin(uint16_t baud)
{
	MPSERIAL.begin(baud);

	delay(100);

	SetPlayMode(MODE_ALL);
	SetVolume(10);
	Stop();
}

void MusicPlayer::Play(int track)
{
	byte len = setCommand(CMD_PLAY, track);
	byte ack = sendCommand(len);
	if(ack != CMD_PLAY) Serial.println("Play Error");
}

void MusicPlayer::Pause()
{
	byte len = setCommand(CMD_PAUSE);
	byte ack = sendCommand(len);
	if(ack != CMD_PAUSE) Serial.println("Pause Error");
}

void MusicPlayer::Stop()
{
	byte len = setCommand(CMD_STOP);
	byte ack = sendCommand(len);
	if(ack != CMD_STOP) Serial.println("Stop Error");
}

void MusicPlayer::Next()
{
	byte len = setCommand(CMD_NEXT);
	byte ack = sendCommand(len);
	if(ack != CMD_NEXT) Serial.println("Next Error");
}

void MusicPlayer::Previous()
{
	byte len = setCommand(CMD_PREV);
	byte ack = sendCommand(len);
	if(ack != CMD_PREV) Serial.println("Previous Error");
}

void MusicPlayer::SetVolume(byte vol)
{
	byte len = setCommand(CMD_VOLUME, vol);
	byte ack = sendCommand(len);
	if(ack != CMD_VOLUME) Serial.println("Volume Error");
}

void MusicPlayer::SetPlayMode(byte mode)
{
	if(mode > 3) {
		Serial.println("Incorrect Mode Number!");
		return ;
	}

	byte len = setCommand(CMD_MODE, mode);
	byte ack = sendCommand(len);
	if(ack != CMD_MODE) Serial.println("SetPlayMode Error");
}

char MusicPlayer::sendCommand(byte len)
{
	MPSERIAL.write(cmdBuf, len);

	while(MPSERIAL.available()==0) ;
	return MPSERIAL.read();
}

byte MusicPlayer::setCommand(byte cmd, int data)
{
	byte ind=0;

	for(int i=0; i<BUFFER_LEN; i++) {
		cmdBuf[i] = 0;
	}

	cmdBuf[ind++] = CMD_START;

	if(cmd == CMD_MODE) {
		cmdBuf[ind++] = 3;
		cmdBuf[ind++] = cmd;
		cmdBuf[ind++] = (data & 0xFF);
	} else if(cmd == CMD_PLAY) {
		cmdBuf[ind++] = 4;
		cmdBuf[ind++] = cmd;
		cmdBuf[ind++] = (data >> 8) & 0xFF;
		cmdBuf[ind++] = (data & 0xFF);
	} else if(cmd == CMD_VOLUME) {
		cmdBuf[ind++] = 3;
		cmdBuf[ind++] = cmd;
		cmdBuf[ind++] = data & 0xFF;
	}else {
		cmdBuf[ind++] = 2;
		cmdBuf[ind++] = cmd;
	}

	cmdBuf[ind++] = CMD_END;

	return ind;
}

int MusicPlayer::GetTrackNumber() {
	int track = -1;

	byte len = setCommand(CMD_GETTRACK);
	byte ack = sendCommand(len);

	if(ack != CMD_GETTRACK) {
		return -1;
	}

	byte h = MPSERIAL.read();
	byte l = MPSERIAL.read();
	track = ((int)h << 8) | l;
	
	return track;
}
