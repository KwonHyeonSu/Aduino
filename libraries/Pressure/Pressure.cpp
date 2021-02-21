#include "Pressure.h"
#include "HardwareSerial.h"


void PressurenTemp::begin() {
	SPI.begin();
	//SPI.setClockDivider(SPI_CLOCK_DIV2);   
	SPI.setBitOrder(MSBFIRST) ;       
     SPI.setClockDivider(SPI_CLOCK_DIV4) ;
     SPI.setDataMode(SPI_MODE0) ;  

	m_nCS = nCSPIN;
	m_Shdn = SHDN;

	pinMode(m_nCS, OUTPUT);
	pinMode(m_Shdn, OUTPUT);

	digitalWrite(m_nCS, HIGH);
	digitalWrite(m_Shdn, HIGH);
}

void PressurenTemp::begin(uint8_t nCS, uint8_t shdn) {
	SPI.begin();
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	/*
	SPI.setBitOrder(MSBFIRST) ;       
     SPI.setClockDivider(SPI_CLOCK_DIV4) ;
     SPI.setDataMode(SPI_MODE0) ;  
	*/
	m_nCS = nCS;
	m_Shdn = shdn;

	pinMode(m_nCS, OUTPUT);
	pinMode(m_Shdn, OUTPUT);

	digitalWrite(m_nCS, HIGH);
	digitalWrite(m_Shdn, HIGH);
}

void PressurenTemp::writeData(byte addr, char data) {
	addr &= 0x7F;

	digitalWrite(m_nCS, LOW);
	delay(1);
	SPI.transfer(addr);
	SPI.transfer(data);
	delay(1);
	digitalWrite(m_nCS, HIGH);
}

char PressurenTemp::readData(byte addr) {
	char data;

	addr |= 0x80;

	digitalWrite(m_nCS, LOW);
	delay(1);
	SPI.transfer(addr);
	data = SPI.transfer((byte)0x0);
	delay(1);
	digitalWrite(m_nCS, HIGH);

	return data;
}

float PressurenTemp::getPressureValue() {
	signed char sia0MSB, sia0LSB;
	signed char sib1MSB, sib1LSB;
	signed char sib2MSB, sib2LSB;
	signed char sic12MSB, sic12LSB;
	signed char sic11MSB, sic11LSB;
	signed char sic22MSB, sic22LSB;
	signed int sia0, sib1, sib2, sic12, sic11, sic22, siPcomp;
	float decPcomp;
	signed long lt1, lt2, lt3, si_c11x1, si_a11, si_c12x2;
	signed long si_a1, si_c22x2, si_a2, si_a1x1, si_y1, si_a2x2;
	unsigned int uiPadc, uiTadc;
	unsigned char uiPH, uiPL, uiTH, uiTL;

	writeData(0x24, 0x00);	// Start Both Conversions
	// write(0x20, 0x00);	// Start Pressure Conversion
	// write(0x22, 0x00);	// Start temperature conversion
	delay(10);	// Typical wait time is 3ms
	
	// Read pressure
	uiPH = readData(PRESH);
	uiPL = readData(PRESL);
	uiTH = readData(TEMPH);
	uiTL = readData(TEMPL);

	
	uiPadc = (unsigned int) uiPH << 8;
	uiPadc += (unsigned int) uiPL & 0x00FF;
	uiTadc = (unsigned int) uiTH << 8;
	uiTadc += (unsigned int) uiTL & 0x00FF;
	
	// Placing Coefficients into 16-bit Variables
	// a0
	sia0MSB = readData(A0MSB);
	sia0LSB = readData(A0LSB);
	sia0 = (signed int) sia0MSB << 8;
	sia0 += (signed int) sia0LSB & 0x00FF;
	
	// b1
	sib1MSB = readData(B1MSB);
	sib1LSB = readData(B1LSB);
	sib1 = (signed int) sib1MSB << 8;
	sib1 += (signed int) sib1LSB & 0x00FF;
	
	// b2
	sib2MSB = readData(B2MSB);
	sib2LSB = readData(B2LSB);
	sib2 = (signed int) sib2MSB << 8;
	sib2 += (signed int) sib2LSB & 0x00FF;
	
	// c12
	sic12MSB = readData(C12MSB);
	sic12LSB = readData(C12LSB);
	sic12 = (signed int) sic12MSB << 8;
	sic12 += (signed int) sic12LSB & 0x00FF;
	
	// c11
	sic11MSB = readData(C11MSB);
	sic11LSB = readData(C11LSB);
	sic11 = (signed int) sic11MSB << 8;
	sic11 += (signed int) sic11LSB & 0x00FF;
	
	// c22
	sic22MSB = readData(C22MSB);
	sic22LSB = readData(C22LSB);
	sic22 = (signed int) sic22MSB << 8;
	sic22 += (signed int) sic22LSB & 0x00FF;
	
	// Coefficient 9 equation compensation
	uiPadc = uiPadc >> 6;
	uiTadc = uiTadc >> 6;
	
	// Step 1 c11x1 = c11 * Padc
	lt1 = (signed long) sic11;
	lt2 = (signed long) uiPadc;
	lt3 = lt1*lt2;
	si_c11x1 = (signed long) lt3;
	
	// Step 2 a11 = b1 + c11x1
	lt1 = ((signed long)sib1)<<14;
	lt2 = (signed long) si_c11x1;
	lt3 = lt1 + lt2;
	si_a11 = (signed long)(lt3>>14);
	
	// Step 3 c12x2 = c12 * Tadc
	lt1 = (signed long) sic12;
	lt2 = (signed long) uiTadc;
	lt3 = lt1*lt2;
	si_c12x2 = (signed long)lt3;
	
	// Step 4 a1 = a11 + c12x2
	lt1 = ((signed long)si_a11<<11);
	lt2 = (signed long)si_c12x2;
	lt3 = lt1 + lt2;
	si_a1 = (signed long) lt3>>11;
	
	// Step 5 c22x2 = c22*Tadc
	lt1 = (signed long)sic22;
	lt2 = (signed long)uiTadc;
	lt3 = lt1 * lt2;
	si_c22x2 = (signed long)(lt3);
	
	// Step 6 a2 = b2 + c22x2
	lt1 = ((signed long)sib2<<15);
	lt2 = ((signed long)si_c22x2>1);
	lt3 = lt1+lt2;
	si_a2 = ((signed long)lt3>>16);
	
	// Step 7 a1x1 = a1 * Padc
	lt1 = (signed long)si_a1;
	lt2 = (signed long)uiPadc;
	lt3 = lt1*lt2;
	si_a1x1 = (signed long)(lt3);
	
	// Step 8 y1 = a0 + a1x1
	lt1 = ((signed long)sia0<<10);
	lt2 = (signed long)si_a1x1;
	lt3 = lt1+lt2;
	si_y1 = ((signed long)lt3>>10);
	
	// Step 9 a2x2 = a2 * Tadc
	lt1 = (signed long)si_a2;
	lt2 = (signed long)uiTadc;
	lt3 = lt1*lt2;
	si_a2x2 = (signed long)(lt3);
	
	// Step 10 pComp = y1 + a2x2
	lt1 = ((signed long)si_y1<<10);
	lt2 = (signed long)si_a2x2;
	lt3 = lt1+lt2;
	
	// Fixed point result with rounding
	//siPcomp = ((signed int)lt3>>13);
	siPcomp = lt3/8192;
	
	// decPcomp is defined as a floating point number
	// Conversion to decimal value from 1023 ADC count value
	// ADC counts are 0 to 1023, pressure is 50 to 115kPa respectively
	decPcomp = ((65.0/1023.0)*siPcomp)+50;
	
	return decPcomp;
}


float PressurenTemp::getAltitudeValue(float pressure)
{
     float h ;

     h = 44330.8 * (1.0 - pow( (pressure/101.325) ,  0.190263 )) ;
     h = h + 80 ;
     return h ;
}