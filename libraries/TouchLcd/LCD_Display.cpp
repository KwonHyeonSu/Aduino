#include "TouchLcd.h"


void TouchLcd::Display_Initial()
{
	SetBkgnTransparency();
	Memory_Clear();
}


void TouchLcd::setModeBitmap() {
	WriteCommand(0x40);
	byte temp = ReadData();
	WriteCommand(0x40);
	WriteData(temp & 0x70);
}

void TouchLcd::SetColor(uint32_t color)
{
	Text_color(color);
}

void TouchLcd::DrawPixel(uint32_t x, uint32_t y, uint32_t color) {

	WriteCommand(0x46);
	WriteData(x);
	WriteCommand(0x47);
	WriteData(x>>8);
	WriteCommand(0x48);
	WriteData(y);
	WriteCommand(0x49);
	WriteData(y>>8);

	WriteCommand(0x02);
	WriteData(color>>8);
	WriteData(color);

	Chk_Busy();
}

void TouchLcd::DrawPixelAutoIncreament(uint32_t color) {
	WriteCommand(0x02);
	WriteData(color>>8);
	WriteData(color);

	Chk_Busy();
}

void TouchLcd::DrawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t lineWidth, uint32_t color)
{
	width -= 1;
	height -= 1;
	uint32_t drawCnt = 0;
	uint32_t temp_x = x, temp_y = y, temp_width = width, temp_height = height;

	SetColor(color);

	for(int drawCnt = 0; drawCnt < lineWidth; drawCnt++)
	{
		Geometric_Coordinate(temp_x++, x+temp_width--, temp_y++, y+temp_height--);
		Draw_square();
		Chk_Busy();
	}
}

void TouchLcd::DrawFillRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
	width -= 1;
	height -= 1;
	SetColor(color);
	Geometric_Coordinate(x, x+width, y, y+height);
	Draw_square_fill();
	Chk_Busy();
}

void TouchLcd::DrawEllipse(uint32_t x, uint32_t y, uint32_t r, uint32_t lineWidth, uint32_t color)
{
	uint32_t drawCnt = 0;

	SetColor(color);

	for(drawCnt = 0; drawCnt < lineWidth; drawCnt++)
	{
		Circle_Coordinate_Radius(x, y, r);
		Draw_circle();
		Chk_Busy();

		r--;
	}
}

void TouchLcd::DrawFillEllipse(uint32_t x, uint32_t y, uint32_t r,  uint32_t color)
{
	SetColor(color);
	Circle_Coordinate_Radius(x, y, r);
	Draw_circle_fill();
	Chk_Busy();
}

void TouchLcd::DrawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color, uint32_t width)
{
	SetColor(color);
	for(int i=0; i<width; i++) {
		Geometric_Coordinate(x1, x2, y1+i, y2+i);
		Draw_line();
		Chk_Busy();
	}
}

void TouchLcd::DrawTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t color) 
{
	uint32_t drawCnt = 0;

	SetColor(color);

	Draw_Triangle_3point_Coordinate(x1, y1, x2, y2, x3, y3);
	Draw_Triangle();
	Chk_Busy();
}

void TouchLcd::DrawFillTriangle(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t x3, uint32_t y3, uint32_t color) {
	SetColor(color);
	Draw_Triangle_3point_Coordinate(x1, y1, x2, y2, x3, y3);
	Draw_Triangle_fill();
	Chk_Busy();
}

void TouchLcd::DrawText(uint32_t x, uint32_t y, const char* string, uint32_t fontsize, uint8_t vertExt, uint8_t horiExt, uint32_t color)
{
	SetColor(color);
	Font_Extension(vertExt, horiExt);
	Text_Mode();

	if(fontsize != FONT_8X16)
	{
		CGROM_Font();
		External_CGROM();
		GT_serial_ROM_select();

		switch(fontsize)
		{
			case FONT_12X24:
				 Font_size_24x24_12x24();
				break;
			case FONT_16X32:
				Font_size_32x32_16x32();
				break;
		}
	}
	else
	{
		uint8_t temp;
		WriteCommand(0x21);	//FNCR0
		temp = ReadData();
		temp &= ~(0x20);
		WriteData(temp);
		Font_size_16x16_8x16();
	}

	Font_Coordinate(x,y);
	Show_String(string, 0);
}

void TouchLcd::DrawFillRectText(uint32_t x, uint32_t y, const char* text, uint8_t lineCnt, uint32_t fillColor, uint32_t textColor, uint32_t width, uint32_t height )
{
	uint8_t maxStringLength = StringMaxCharCount(text, lineCnt);
	uint32_t strWidth = 8*maxStringLength;
	uint32_t strHeight = 16* lineCnt;

	uint32_t rectWidth = strWidth;
	uint32_t rectHeight = 0; //strHeight + 10;
	uint32_t font_x = 0;
	uint32_t font_y = 0;
	uint8_t loopCnt = 0;
	uint8_t idx = 0;
	uint8_t temp = 0, sum = 0;

	if(height == 0)
		rectHeight = strHeight + 10;
	else
		rectHeight = height;

	if(width == 0)
		rectWidth = strWidth + 10;
	else
		rectWidth = width;

	DrawFillRect(x, y, rectWidth, rectHeight, fillColor);

	for(loopCnt = 0; loopCnt < lineCnt; loopCnt++)
	{
		idx = GetNextString(text,  idx);
		temp = idx - sum - 1;
		sum = idx;

		strWidth = temp * 8;
		font_x = x  + ((rectWidth - strWidth) / 2);
		font_y = (y + (16 * loopCnt)) + ((rectHeight - strHeight) / 2-1);
		DrawText(font_x, font_y, (const char*)strBuf, FONT_8X16, VERTICAL_EXT1, HORIZONTAL_EXT1, textColor);
	}
}

void TouchLcd::DrawButton(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, const char* strText) {
	DrawRect(x, y, w, h, 1, COLOR_BLACK);
	DrawRect(x+1, y+1, w-2, h-2, 2, color);
	DrawRect(x+1, y+1, w-3, h-3, 2, color + RGB(27,40,40));
	DrawRect(x+3, y+3, w-4, h-4, 2, color - RGB(30,30,10));
	DrawFillRectText(x+3, y+3, strText, 1, color, COLOR_BLACK, w-6, h-6);
}

void TouchLcd::DrawMenuButton(uint32_t x, uint32_t y, uint32_t color) {
	//DrawEllipse(x, y, 52, 2, COLOR_BLACK);
	DrawEllipse(x, y, 51, 2, color);
	DrawEllipse(x-1, y-1, 50, 3, color + RGB(34,34,34));
	DrawEllipse(x+1, y+1, 50, 3, color - RGB(44,44,44));
	DrawFillEllipse(x, y, 49, color);
	DrawEllipse(x-1, y-1, 35, 2, COLOR_WHITE);
	DrawEllipse(x+1, y+1, 35, 2, RGB(192,192,192));
	DrawFillEllipse(x, y, 35, COLOR_WHITE);
}

uint8_t TouchLcd::StringMaxCharCount(const char* text, uint8_t lineCnt)
{
	uint8_t charCnt = 0, charCntTemp = 0;
	uint8_t loopCnt = 0;
	uint8_t idx = 0;

	for(loopCnt = 0; loopCnt < lineCnt; loopCnt++)
	{
		while(*(text+idx) != '\n' && *(text+idx) != '\0')
		{
			++charCntTemp;
			++idx;
		}

		if(charCnt < charCntTemp)
		{
			charCnt = charCntTemp;
		}

		idx++;
		charCntTemp = 0;
	}

	return charCnt;
}

uint8_t TouchLcd::GetNextString(const char* text, uint8_t idx)
{
	uint8_t cnt = 0;

	memset(strBuf, 0, sizeof(strBuf)/sizeof(uint8_t));

	while(*(text + idx) != '\n' && *(text + idx) != '\0')
	{
		strBuf[cnt++] = *(text + idx++);
	}

	strBuf[cnt] = '\0';

	return idx+1;
}

