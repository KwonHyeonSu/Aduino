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

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//#include "Wire.h"
#include "SoftI2CMaster.h"
#include "Adafruit_GFX.h"

#define LED_ON 1
#define LED_OFF 0

#define LED_RED 1
#define LED_YELLOW 2
#define LED_GREEN 3
 


#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

#define HT16K33_CMD_BRIGHTNESS 0x0E

#define SEVENSEG_DIGITS 5


// this is the raw HT16K33 controller
class SoftI2C_LEDBackpack {
 public:
  SoftI2C_LEDBackpack(void);
  SoftI2C_LEDBackpack(uint8_t sclPin, uint8_t sdaPin);
  void begin(uint8_t _addr);
  void setBrightness(uint8_t b);
  void blinkRate(uint8_t b);
  void writeDisplay(void);
  void clear(void);

  uint16_t displaybuffer[8]; 

  void init(uint8_t a);
  
  SoftI2CMaster softi2c;
 private:
  uint8_t i2c_addr;
};

class SoftI2C_8x8matrix : public SoftI2C_LEDBackpack, public Adafruit_GFX {
 public:
  SoftI2C_8x8matrix(void);
  SoftI2C_8x8matrix(uint8_t sclPin, uint8_t sdaPin);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

 private:
};

class SoftI2C_BicolorMatrix : public SoftI2C_LEDBackpack, public Adafruit_GFX {
 public:
  SoftI2C_BicolorMatrix(void);
  SoftI2C_BicolorMatrix(uint8_t sclPin, uint8_t sdaPin);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

 private:
};
