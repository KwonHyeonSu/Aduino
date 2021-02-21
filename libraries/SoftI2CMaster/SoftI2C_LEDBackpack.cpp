/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED Matrix backpacks 
  ----> http://www.adafruit.com/products/
  ----> http://www.adafruit.com/products/

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

//#include <Wire.h>
#include "SoftI2C_LEDBackpack.h"
//#include "Adafruit_GFX.h"

static const uint8_t numbertable[] = { 
	0x3F, /* 0 */
	0x06, /* 1 */
	0x5B, /* 2 */
	0x4F, /* 3 */
	0x66, /* 4 */
	0x6D, /* 5 */
	0x7D, /* 6 */
	0x07, /* 7 */
	0x7F, /* 8 */
	0x6F, /* 9 */
	0x77, /* a */
	0x7C, /* b */
	0x39, /* C */
	0x5E, /* d */
	0x79, /* E */
	0x71, /* F */
};

void SoftI2C_LEDBackpack::setBrightness(uint8_t b) {
  if (b > 15) b = 15;
  softi2c.beginTransmission(i2c_addr);
  softi2c.write(0xE0 | b);
  softi2c.endTransmission();  
}

void SoftI2C_LEDBackpack::blinkRate(uint8_t b) {
  softi2c.beginTransmission(i2c_addr);
  if (b > 3) b = 0; // turn off if not sure
  
  softi2c.write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)); 
  softi2c.endTransmission();
}

SoftI2C_LEDBackpack::SoftI2C_LEDBackpack(void) {
  softi2c = SoftI2CMaster();
}

SoftI2C_LEDBackpack::SoftI2C_LEDBackpack(uint8_t sclPin, uint8_t sdaPin) {
  softi2c = SoftI2CMaster(sclPin, sdaPin);
}

void SoftI2C_LEDBackpack::begin(uint8_t _addr = 0x70) {
  i2c_addr = _addr;

  //softi2c.begin();

  softi2c.beginTransmission(i2c_addr);
  softi2c.write(0x21);  // turn on oscillator
  softi2c.endTransmission();
  blinkRate(HT16K33_BLINK_OFF);
  
  setBrightness(15); // max brightness
}

void SoftI2C_LEDBackpack::writeDisplay(void) {
  softi2c.beginTransmission(i2c_addr);
  softi2c.write((uint8_t)0x00); // start at address $00

  for (uint8_t i=0; i<8; i++) { 
    softi2c.write((uint8_t)(displaybuffer[i] & 0xFF));    
    softi2c.write((uint8_t)(displaybuffer[i] >> 8));   
  }

  softi2c.endTransmission();  
}

void SoftI2C_LEDBackpack::clear(void) {
  for (uint8_t i=0; i<8; i++) {
    displaybuffer[i] = 0;
  }
}

SoftI2C_8x8matrix::SoftI2C_8x8matrix(void) {
  constructor(8, 8);
}

SoftI2C_8x8matrix::SoftI2C_8x8matrix(uint8_t sclPin, uint8_t sdaPin) {
  softi2c = SoftI2CMaster(sclPin, sdaPin);
  constructor(8, 8);
}

void SoftI2C_8x8matrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((y < 0) || (y >= 8)) return;
  if ((x < 0) || (x >= 8)) return;

 // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    swap(x, y);
    x = 8 - x - 1;
    break;
  case 2:
    x = 8 - x - 1;
    y = 8 - y - 1;
    break;
  case 3:
    swap(x, y);
    y = 8 - y - 1;
    break;
  }

  // wrap around the x
  x += 7;
  x %= 8;


  if (color) {
    displaybuffer[y] |= 1 << x;
  } else {
    displaybuffer[y] &= ~(1 << x);
  }
}


SoftI2C_BicolorMatrix::SoftI2C_BicolorMatrix(void) {
  constructor(8, 8);
}

SoftI2C_BicolorMatrix::SoftI2C_BicolorMatrix(uint8_t sclPin, uint8_t sdaPin) {
  softi2c = SoftI2CMaster(sclPin, sdaPin);
  constructor(8, 8);
}

void SoftI2C_BicolorMatrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((y < 0) || (y >= 8)) return;
  if ((x < 0) || (x >= 8)) return;

  switch (getRotation()) {
  case 1:
    swap(x, y);
    x = 8 - x - 1;
    break;
  case 2:
    x = 8 - x - 1;
    y = 8 - y - 1;
    break;
  case 3:
    swap(x, y);
    y = 8 - y - 1;
    break;
  }

  if (color == LED_GREEN) {
    displaybuffer[y] |= 1 << x;
  } else if (color == LED_RED) {
    displaybuffer[y] |= 1 << (x+8);
  } else if (color == LED_YELLOW) {
    displaybuffer[y] |= (1 << (x+8)) | (1 << x);
  } else if (color == LED_OFF) {
    displaybuffer[y] &= ~(1 << x) & ~(1 << (x+8));
  }
}