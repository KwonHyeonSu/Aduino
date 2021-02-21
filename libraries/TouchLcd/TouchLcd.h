#ifndef __TOUCHLCD_H__
#define __TOUCHLCD_H__

#include <inttypes.h>
#include "SPI.h"
#include "Arduino.h"

#define Color65k
//#define Color256


#define LCD_WIDTH				480
#define LCD_HEIGHT				272

/* RGB Value Macro definitions */
#define GetRValue(color)	((color & 0xf800) >> 11)
#define GetGValue(color)	((color & 0x07e0) >> 5)
#define GetBValue(color)	(color & 0x1f)
#define RGB(r,g,b)			(((map(r, 0x0, 0xff, 0x0, 0x1f) << 11) & 0xf800) |\
						  ((map(g, 0x0, 0xff, 0x0, 0x3f) << 5) & 0x07e0) | \
						  ((map(b, 0x0, 0xff, 0x0, 0x1f) << 0) & 0x1f))



/* Font size definitions */
#define FONT_8X16	0
#define FONT_12X24	1
#define FONT_16X32	2

/* Vertical Extension */
#define VERTICAL_EXT1	(0x0)
#define VERTICAL_EXT2	(0x1)
#define VERTICAL_EXT3	(0x2)
#define VERTICAL_EXT4	(0x3)

/* Horizontal  Extension */
#define HORIZONTAL_EXT1		(0x0 << 2)
#define HORIZONTAL_EXT2		(0x1 << 2)
#define HORIZONTAL_EXT3		(0x2 << 2)
#define HORIZONTAL_EXT4		(0x3 << 2)

/* RGB color definitions */
#define COLOR_BLACK	0x0000
#define COLOR_WHITE		0xffff
#define COLOR_RED		0xf800
#define COLOR_GREEN	0x07e0
#define COLOR_BLUE		0x001f
#define COLOR_YELLOW	COLOR_RED | COLOR_GREEN
#define COLOR_CYAN		COLOR_GREEN | COLOR_BLUE
#define COLOR_PURPLE	COLOR_RED | COLOR_BLUE


#define POS_X	0
#define POS_Y	1


/* LCD SPI Chip Select(Slave Select) */
//const int CSS = 78;

class TouchLcd
{
	private:
		uint8_t m_css;
		char strBuf[256];
		unsigned int  ADC_value_X, ADC_value_Y;

	public:
		void begin(uint8_t css);
		void LCD_Initial(void);
		void LcdSetArea(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2);
		void LCD_DisplayOn(void);
		uint8_t ReadData(void);
		void Memory_Clear(void);
		void Chk_Busy(void);
		uint8_t LCD_StatusRead(void);
		void LCD_Geometric_Test(void);
		void Text_color(uint32_t color);
		void Internal_CGROM(void);
		void Font_size_16x16_8x16(void);
		void Font_Coordinate(uint32_t X,uint32_t Y);
		void Show_String(const char *str, uint32_t n);
		void Text_Mode(void);
		void Draw_Triangle_3point_Coordinate(uint32_t P1_X,uint32_t P1_Y,uint32_t P2_X,uint32_t P2_Y,uint32_t P3_X,uint32_t P3_Y);
		void Draw_Triangle(void);
		void Draw_Triangle_fill(void);
		void Circle_Coordinate_Radius(uint32_t X,uint32_t Y,uint32_t R);
		void Draw_circle(void);
		void Draw_circle_fill(void);
		void Geometric_Coordinate(uint32_t XL,uint32_t XR,uint32_t YT,uint32_t YB);
		void Draw_square(void);
		void Draw_square_fill(void);
		void Circle_Square_Coordinate_setting(uint32_t XL,uint32_t XR ,uint32_t YT ,uint32_t YB ,uint32_t ELL_A,uint32_t ELL_B);
		void Draw_Circle_Square(void);
		void Ellipse_Coordinate_setting(uint32_t X,uint32_t Y,uint32_t ELL_A,uint32_t ELL_B);
		void Draw_Ellipse(void);
		void Draw_Ellipse_fill(void);
		void Draw_Ellipse_Curve_part(uint8_t setx);
		void Draw_Ellipse_Curve(void);
		void Draw_Ellipse_Curve_Fill(void);
		void Draw_line(void);
		void PWM1_enable(void);
		void PWM1_fnuction_sel(void);
		void PWM1_clock_ratio(uint8_t setx);
		void PWM1_duty_cycle(uint8_t setx);
		void CGROM_Font(void);
		void External_CGROM(void);
		void Font_size_24x24_12x24(void);
		void GT_serial_ROM_select(void);
		void Font_code_GB2312(void);
		void Font_Standard(void);
		void Serial_ROM_select0(void);
		void Background_color(uint32_t color);
		void Font_size_32x32_16x32(void);
		void LCD_PLL_ini(void);
		void WriteCommand(unsigned char cmd);
		void WriteData(unsigned char data);
		void Font_Extension(uint8_t vext, uint8_t hext);
		void SetBkgnTransparency(void);

		// LCD_Display
		//void Initialize();
		void Display_Initial();
		void setModeBitmap();
		void DrawPixel(uint32_t x, uint32_t y, uint32_t color);
		void DrawPixelAutoIncreament(uint32_t color);
		void DrawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t lineWidth, uint32_t color);
		void DrawFillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
		void DrawEllipse(uint32_t x, uint32_t y, uint32_t r, uint32_t lineWidth, uint32_t color);
		void DrawFillEllipse(uint32_t x, uint32_t y, uint32_t r, uint32_t color);
		void DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color, uint32_t width=1);
		void DrawTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t color);
		void DrawFillTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t color);
		void DrawText(uint32_t x, uint32_t y, const char* string, uint32_t fontsize = FONT_8X16, uint8_t vertExt = VERTICAL_EXT1, uint8_t horiExt = HORIZONTAL_EXT1, uint32_t color = RGB(255,255,255));
		void SetColor(uint32_t color);
		void DrawFillRectText(uint32_t x, uint32_t y, const char* text, uint8_t lineCnt, uint32_t fillColor, uint32_t textColor, uint32_t width=0, uint32_t height = 0);
		uint8_t StringMaxCharCount(const char* text, uint8_t lineCnt);
		uint8_t GetNextString(const char* text, uint8_t idx);

		void DrawButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, const char* strText);
		void DrawMenuButton(uint32_t x, uint32_t y, uint32_t color);

		// LCD_Touch
		//void begin();
		void Touch_Initial();
		void Touch_Auto_Mode(void);
		void Touch_ADC_Clock(void);
		void Touch_Enable_TP(void);
		uint8_t Touch_Detect(void);
		unsigned char Touch_Status(void);
		void clear_TP_interrupt(void);
		void Access_TP_coordinate(void);
		unsigned int Get_X_ADC_Value_10bit(void);
		unsigned char ADC_X(void);
		unsigned int Get_Y_ADC_Value_10bit(void);
		unsigned char ADC_Y(void);
                unsigned char ADC_XY(void);
		//void MidValueCalculate(void);
		//void Get_XY_Calibration(void);
		void Trace_XY_Value(void);
		void Print_Hex(unsigned char buf);
		//void Trace_coordinate(void);
		void Print_Decimal(unsigned int buffer);
		uint32_t GetXYValue(uint8_t kind_xy);
};

//extern LCD lcd;

#endif
