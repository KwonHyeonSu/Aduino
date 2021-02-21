#ifndef __PRESSURE_H__
#define __PRESSURE_H__

#include "Arduino.h"
#include <inttypes.h>
#include <SPI.h>

#define nCSPIN	13
#define SHDN	6

#define PRESH	0x00	// 80
#define	PRESL	0x02	// 82
#define	TEMPH	0x04	// 84
#define	TEMPL	0x06	// 86

#define A0MSB	0x08	// 88
#define A0LSB	0x0A	// 8A
#define B1MSB	0x0C	// 8C
#define B1LSB	0x0E	// 8E
#define	B2MSB	0x10	// 90
#define B2LSB	0x12	// 92
#define C12MSB	0x14	// 94
#define	C12LSB	0x16	// 96
#define	C11MSB	0x18	// 98
#define C11LSB	0x1A	// 9A
#define C22MSB	0x1C	// 9C
#define C22LSB	0x1E	// 9E


class PressurenTemp
{
private:
	uint8_t m_nCS;
	uint8_t m_Shdn;

	void writeData(byte addr, char data);
	char readData(byte addr);

public:
	void begin();
	void begin(uint8_t nCS, uint8_t shdn);
	float getPressureValue();
	float getAltitudeValue(float pressure);
};

#endif