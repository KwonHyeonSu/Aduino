#include "Si4703.h"
#include <Wire.h>

void Si4703::gotoChannel(float channel) 
{
	int newCh=0;
	newCh = (int)(channel*100);
	newCh -= 8750;


#ifdef IN_EUROPE
	newCh /= 10;
#else
	newCh /= 20;
#endif

	Serial.println(newCh);

	readRegisters();
	registers[CHANNEL] &= 0xFE00;
	registers[CHANNEL] |= newCh;
	registers[CHANNEL] |= (1<<TUNE);
	updateRegisters();

	delay(60);

	Serial.println("Tuning..");
	while(1) {
		readRegisters();
		if((registers[STATUSRSSI] & (1<<STC)) != 0) break;
	}

	readRegisters();
	registers[CHANNEL] &= ~(1<<TUNE);
	updateRegisters();

	Serial.println("Waiting..");
	while(1) {
		readRegisters();
		if((registers[STATUSRSSI] & (1<<STC)) == 0) break;
	}
}

float Si4703::readChannel(void) 
{
	readRegisters();
	int ch = registers[READCHAN] & 0x3FF;

#ifdef IN_EUROPE
	ch *= 10;
#else
	ch *= 20;
#endif

	ch += 8750;
	return ((float)ch /100);
}

byte Si4703::seek(byte dir) 
{
	readRegisters();
	Serial.println(readChannel());

	registers[POWERCFG] &= ~(1<<SKMODE);

	if(dir == SEEK_DOWN) registers[POWERCFG] &= ~(1<<SEEKUP);
	else registers[POWERCFG] |= (1<<SEEKUP);

	registers[POWERCFG] |= (1<<SEEK);
	updateRegisters();

	while(1) {
		readRegisters();
		if((registers[STATUSRSSI] & (1<<STC)) != 0) break;

		Serial.print("Trying : ");
		Serial.println(readChannel());
	}

	readRegisters();

	int valueSFBL = registers[STATUSRSSI] & (1<<SFBL);
	registers[POWERCFG] &= ~(1<<SEEK);
	updateRegisters();

	Serial.println("Waiting..");
	while(1) {
		readRegisters();
		if((registers[STATUSRSSI] & (1<<STC)) == 0) break;
	}

	if(valueSFBL) {
		Serial.println("Seek limit hit!");
		return (FAIL);
	}

	Serial.println("Seek complete.");
	return (SUCCESS);
}

void Si4703::begin(int rs) 
{
	Serial.println("Initialize I2C and Si4703");

	rsPin = rs;
	pinMode(rs, OUTPUT);

	digitalWrite(rs, LOW);
	delay(1);
	digitalWrite(rs, HIGH);
	delay(1);
	
	Wire.begin();

	readRegisters();
	registers[0x07] = 0x8100;
	updateRegisters();

	delay(500);

	readRegisters();
	registers[POWERCFG] = 0x4001;
	registers[SYSCONFIG1] |= (1<<RDS);

	registers[SYSCONFIG2] &= ~((1<<SPACE1) | (1<<SPACE0));

	registers[SYSCONFIG2] &= 0xFFF0;
	registers[SYSCONFIG2] |= 0x0003;
	updateRegisters();

	delay(110);
}

byte Si4703::updateRegisters()
{
	Wire.beginTransmission(SI4703_ADDR);

	for(int regSpot=0x02; regSpot<0x08; regSpot++) {
		byte highByte = registers[regSpot] >> 8;
		byte lowByte = (byte)(registers[regSpot] & 0x00FF);

		Wire.write(highByte);
		Wire.write(lowByte);
	}

	byte ack = Wire.endTransmission();
	if(ack != 0) {
		Serial.print("Wire write fail : ");
		Serial.println(ack);
		return (FAIL);
	}

	return (SUCCESS);
}

void Si4703::readRegisters()
{
	Wire.requestFrom(SI4703_ADDR, 32);
	while(Wire.available() < 32) ;

	for(int x=0x0A; ; x++) {
		if(x==0x10) x=0;

		registers[x] = Wire.read() << 8;
		registers[x] |= Wire.read();

		if(x == 0x09) break;
	}
}

void Si4703::printRegisters()
{
	readRegisters();

	for(int i=0; i<16; i++) {
		sprintf(printBuffer, "Reg 0x%02X = 0x%04X", i, registers[i]);
		Serial.println(printBuffer);
	}
}

void Si4703::mute(byte bMute) 
{
	readRegisters();
	if(bMute) registers[POWERCFG] &= ~(1<<DMUTE);
	else registers[POWERCFG] |= (1<<DMUTE);
	updateRegisters();
}

void Si4703::setVolume(byte vol)
{
	if(vol > 16) vol = 16;
	
	readRegisters();
	registers[SYSCONFIG2] &= 0xFFF0;
	registers[SYSCONFIG2] |= vol;
	updateRegisters();
}