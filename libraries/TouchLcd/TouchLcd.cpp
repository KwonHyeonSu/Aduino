#include "TouchLcd.h"

//LCD lcd;

void TouchLcd::begin(uint8_t css) {
	m_css = css;

	Serial.println("Init Start");
	SPI.begin();
	delay(100);

	LCD_Initial();
	Serial.println("Init Complete");
	delay(10);
	Display_Initial();
	delay(10);
	Touch_Initial();
	delay(10);

}

void TouchLcd::LCD_PLL_ini(void)
{
	WriteCommand(0x88);
	WriteData(0x0a);
	delay(30);
	WriteCommand(0x89);
	WriteData(0x02);
	delay(30);
	WriteCommand(0x01);
	WriteData(0x01);
	WriteData(0x00);
	delay(30);
}

void TouchLcd::LCD_Initial(void)
{
	pinMode(m_css, OUTPUT);
	LCD_PLL_ini();                  //PLL��
	WriteCommand(0x10);
	WriteData(0x0C);                //65Kɫ,8bit
	WriteCommand(0x04);
	WriteData(0x82);

	
	Serial.println("ClockSet()");
	//Horizontal set
	WriteCommand(0x14);             //Horizontal Display Width Setting
	WriteData(0x3B);                //Horizontal display width(pixels) = (HDWR + 1)*8
	WriteCommand(0x15);
	WriteCommand(0x00);
	WriteCommand(0x16);             //HNDR//Horizontal Non-Display Period Bit[4:0]
	WriteData(0x05);                //HSTR//HSYNC Start Position[4:0]
	WriteCommand(0x17);
	WriteData(0x00);
	WriteCommand(0x18);             //HPWR//HSYNC Polarity ,The period width of HSYNC.
	WriteData(0x00);
	//Vertical set
	WriteCommand(0x19);             //VDHR0 //Vertical Display Height Bit [7:0]
	WriteData(0x0f);                //Vertical pixels = VDHR + 1
	WriteCommand(0x1a);             //VDHR1 //Vertical Display Height Bit [8]
	WriteData(0x01);                //Vertical pixels = VDHR + 1
	WriteCommand(0x1b);             //VNDR0 //Vertical Non-Display Period Bit [7:0]
	WriteData(0x02);
	WriteCommand(0x1c);             //VNDR1 //Vertical Non-Display Period Bit [8]
	WriteData(0x00);
	WriteCommand(0x1d);             //VSTR0 //VSYNC Start Position[7:0]
	WriteData(0x0B);
	WriteCommand(0x1e);             //VSTR1 //VSYNC Start Position[8]
	WriteData(0x00);
	WriteCommand(0x1f);             //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]
	WriteData(0x00);

	WriteCommand(0x06);
	WriteData(0x03);
	LcdSetArea(0, 0, 479, 271);
	Memory_Clear();
	LCD_DisplayOn();

	PWM1_enable();                              // Enable PWM control backlight
	PWM1_fnuction_sel();
	PWM1_clock_ratio(0x0a);
	PWM1_duty_cycle(0xff);
	delay(100);


	Text_color(COLOR_WHITE);
	Geometric_Coordinate(0, LCD_WIDTH, 0, LCD_HEIGHT);
	Draw_square_fill();
	Chk_Busy();
}

void TouchLcd::SetBkgnTransparency(void)
{
	WriteCommand(0x22);
	 uint8_t temp = ReadData();
	temp |= 0x40;
	WriteCommand(0x22);
	WriteData(temp);
}

void TouchLcd::LcdSetArea(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
	WriteCommand(0x30);
	WriteData(x1);
	WriteCommand(0x31);
	WriteData(x1>>8);
	WriteCommand(0x34);
	WriteData(x2);
	WriteCommand(0x35);
	WriteData(x2>>8);
	WriteCommand(0x32);
	WriteData(y1);
	WriteCommand(0x33);
	WriteData(y1>>8);
	WriteCommand(0x36);
	WriteData(y2);
	WriteCommand(0x37);
	WriteData(y2>>8);
}

void TouchLcd::LCD_Geometric_Test(void)
{
	Text_color(COLOR_WHITE);
	Geometric_Coordinate(0,479,0,271);
	Draw_square_fill();
	Chk_Busy();
	Text_color(COLOR_GREEN);
//	Geometric_Coordinate(0,479,0,271);
	Draw_square();
	Chk_Busy();
}

void TouchLcd::Font_size_32x32_16x32(void)
{
	uint8_t temp;
	WriteCommand(0x2E);//FNCR1
	temp = ReadData();
	temp &= 0x3f;
	temp |= 0x80;
	WriteData(temp);
}

void TouchLcd::Background_color(uint32_t color)
{
	#ifdef Color65k
		WriteCommand(0x60);
		WriteData((color&0xf800)>>11);

		WriteCommand(0x61);
		WriteData((color&0x07e0)>>5);

		WriteCommand(0x62);
		WriteData((color&0x001f));
	#endif

	#ifdef Color256
		WriteCommand(0x60);
		WriteData((color&0xf800)>>13);

		WriteCommand(0x61);
		WriteData((color&0x07e0)>>8);

		WriteCommand(0x62);
		WriteData((color&0x001f)>>3);
	#endif
}


void TouchLcd::Serial_ROM_select0(void)
{
	uint8_t temp;
	WriteCommand(0x05);	//PWRR
	temp = ReadData();
	temp &=0x7f;
	WriteData(temp);
}

void TouchLcd::Font_Standard(void)
{
	uint8_t temp;
	WriteCommand(0x2F);	//FNCR1
	temp = ReadData();
	temp &= 0xFC;
	WriteData(temp);
}

void TouchLcd::Font_code_GB2312(void)
{
	uint8_t temp;
	WriteCommand(0x2F);	//FNCR1
	temp = ReadData();
	temp &= 0xE3;
	WriteData(temp);
}

void TouchLcd::GT_serial_ROM_select(void)
{
	uint8_t temp;
	WriteCommand(0x2F);	//FNCR1
	temp = ReadData();
	temp &= 0x1f;
	temp |= 0x80;
	WriteData(temp);
}

void TouchLcd::Font_size_24x24_12x24(void)
{
	uint8_t temp;
	WriteCommand(0x2E);	//FNCR1
	temp = ReadData();
	temp &= 0x3f;
	temp |= 0x40;
	WriteData(temp);
}

void TouchLcd::External_CGROM(void)
{
	uint8_t temp;
	WriteCommand(0x21);	//FNCR0
	temp = ReadData();
	temp |= 0x20 ;
	WriteData(temp);
}

void TouchLcd::CGROM_Font(void)
{
	uint8_t temp;
	WriteCommand(0x21);
	temp = ReadData();
	temp &= 0x7f;
	WriteData(temp);
}

void TouchLcd::PWM1_duty_cycle(uint8_t setx) //bit0~7
{
	WriteCommand(0x8b);	//PTNO
	WriteData(setx);
}

void TouchLcd::PWM1_clock_ratio(uint8_t setx) //bit0~3
{
	uint8_t temp,temp1;
	temp1= setx&0x0f;
	WriteCommand(0x8a);	//MCLR
	temp = ReadData();
	temp &= 0xf0;
	temp |= temp1 ;
	WriteData(temp);
}

void TouchLcd::PWM1_fnuction_sel(void)
{
	uint8_t temp;
	WriteCommand(0x8a);	//MCLR
	temp = ReadData();
	temp &= 0xef ;
	WriteData(temp);
}

void TouchLcd::PWM1_enable(void)
{
	uint8_t temp;
	WriteCommand(0x8a);	//MCLR
	temp = ReadData();
	temp |= 0x80 ;
	WriteData(temp);
}

void TouchLcd::Draw_line(void)
{
	WriteCommand(0x90);
	WriteData(0x00);
	WriteData(0x80);
}

void TouchLcd::Draw_Ellipse_Curve_Fill(void)
{
	uint8_t temp;
	WriteCommand(0xa0);//DCR
	temp = ReadData();
	temp &=0x1F;
	temp |= 0xD0;
	WriteCommand(0xa0);
	WriteData(temp);
}

void TouchLcd::Draw_Ellipse_Curve(void)
{
	uint8_t temp;
	WriteCommand(0xa0);
	temp = ReadData();
	temp &= 0x1f;
	temp |= 0x90;
	WriteCommand(0xa0);
	WriteData(temp);
}

void TouchLcd::Draw_Ellipse_Curve_part(uint8_t setx)
{
	uint8_t temp,temp1;
	WriteCommand(0xA0);//DCR
	temp = ReadData();
	temp1 = setx&0x03;
	temp &=0xfc;
	temp |=temp1;
	WriteCommand(0xA0);
	WriteData(temp);
}

void TouchLcd::Draw_Ellipse_fill(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x40);
	WriteData(0xC0);
}

void TouchLcd::Ellipse_Coordinate_setting(uint32_t X,uint32_t Y,uint32_t ELL_A,uint32_t ELL_B)
{
	WriteCommand(0xA5);
	WriteData(X);
	WriteCommand(0xA6);
	WriteData(X>>8);

	WriteCommand(0xA7);
	WriteData(Y);
	WriteCommand(0xA8);
	WriteData(Y>>8);

	WriteCommand(0xA1);
	WriteData(ELL_A);
	WriteCommand(0xA2);
	WriteData(ELL_A>>8);

	WriteCommand(0xA3);
	WriteData(ELL_B);
	WriteCommand(0xA4);
	WriteData(ELL_B>>8);
}

void TouchLcd::Draw_Circle_Square(void)
{
	WriteCommand(0xA0);//DCR
	WriteData(0x20);
	WriteData(0xA0);
}

void TouchLcd::Circle_Square_Coordinate_setting(uint32_t XL,uint32_t XR ,uint32_t YT ,uint32_t YB ,uint32_t ELL_A,uint32_t ELL_B)
{
	WriteCommand(0x91);
	WriteData(XL);
	WriteCommand(0x92);
	WriteData(XL>>8);

	WriteCommand(0x95);
	WriteData(XR);
	WriteCommand(0x96);
	WriteData(XR>>8);

	WriteCommand(0x93);
	WriteData(YT);
	WriteCommand(0x94);
	WriteData(YT>>8);

	WriteCommand(0x97);
	WriteData(YB);
	WriteCommand(0x98);
	WriteData(YB>>8);

	WriteCommand(0xA1);
	WriteData(ELL_A);
	WriteCommand(0xA2);
	WriteData(ELL_A>>8);

	WriteCommand(0xA3);
	WriteData(ELL_B);
	WriteCommand(0xA4);
	WriteData(ELL_B>>8);
}

void TouchLcd::Draw_square_fill(void)
{
	WriteCommand(0x90);
	WriteData(0x10);
	WriteData(0xb0);
}

void TouchLcd::Draw_square(void)
{
	WriteCommand(0x90);
	WriteData(0x10);
	WriteData(0x90);
}


void TouchLcd::Geometric_Coordinate(uint32_t XL,uint32_t XR,uint32_t YT,uint32_t YB)
{
	WriteCommand(0x91);
	WriteData(XL);
	WriteCommand(0x92);
	WriteData(XL>>8);
	WriteCommand(0x95);
	WriteData(XR);
	WriteCommand(0x96);
	WriteData(XR>>8);
	WriteCommand(0x93);
	WriteData(YT);
	WriteCommand(0x94);
	WriteData(YT>>8);
	WriteCommand(0x97);
	WriteData(YB);
	WriteCommand(0x98);
	WriteData(YB>>8);
}

void TouchLcd::Draw_circle_fill(void)
{
	WriteCommand(0x90);
	WriteData(0x60);
}

void TouchLcd::Draw_circle(void)
{
	WriteCommand(0x90);
	WriteData(0x40);
}

void TouchLcd::Circle_Coordinate_Radius(uint32_t X,uint32_t Y,uint32_t R)
{
	WriteCommand(0x99);
	WriteData(X);
	WriteCommand(0x9a);
	WriteData(X>>8);
	WriteCommand(0x9b);
	WriteData(Y);
	WriteCommand(0x9c);
	WriteData(Y>>8);
	WriteCommand(0x9d);
	WriteData(R);
}

void TouchLcd::Draw_Ellipse(void)
{
	WriteCommand(0xa0);
	WriteData(0x00);
	WriteData(0x80);
}


void TouchLcd::Draw_Triangle_fill(void)
{
	WriteCommand(0x90);
	WriteData(0x01);
	WriteData(0xa1);
}

void TouchLcd::Draw_Triangle(void)
{
	WriteCommand(0x90);
	WriteData(0x00);
	WriteData(0x81);
}

void TouchLcd::Draw_Triangle_3point_Coordinate(uint32_t P1_X,uint32_t P1_Y,uint32_t P2_X,uint32_t P2_Y,uint32_t P3_X,uint32_t P3_Y)
{
    WriteCommand(0x91);
    WriteData(P1_X);
    WriteCommand(0x92);
    WriteData(P1_X>>8);

    WriteCommand(0x93);
    WriteData(P1_Y);
    WriteCommand(0x94);
    WriteData(P1_Y>>8);

    WriteCommand(0x95);
    WriteData(P2_X);
    WriteCommand(0x96);
    WriteData(P2_X>>8);

    WriteCommand(0x97);
    WriteData(P2_Y);
    WriteCommand(0x98);
    WriteData(P2_Y>>8);

    WriteCommand(0xA9);
    WriteData(P3_X);
    WriteCommand(0xAA);
    WriteData(P3_X>>8);

    WriteCommand(0xAB);
    WriteData(P3_Y);
    WriteCommand(0xAC);
    WriteData(P3_Y>>8);
}

void TouchLcd::Text_Mode(void)
{
	WriteCommand(0x40);	//MWCR0
	WriteData(0x80);
}


void TouchLcd::Show_String(const char *str, uint32_t n)
{
	WriteCommand(0x02);

	while(*str != '\0')
	{
		WriteData(*str);
		++str;
		delay(1);
	}
	delay(n);
}

void TouchLcd::Font_Coordinate(uint32_t X,uint32_t Y)
{
	WriteCommand(0x2A);//HOFS0
	WriteData(X);

	WriteCommand(0x2B);//HOFS1
	WriteData(X>>8);


	WriteCommand(0x2C);//VOFS0
	WriteData(Y);

	WriteCommand(0x2D);//VOFS1
	WriteData(Y>>8);
}

void TouchLcd::Font_size_16x16_8x16(void)
{
	uint8_t temp;
	WriteCommand(0x2E);	//FNCR1
	temp = ReadData();
	temp &= 0x3f;
	WriteData(temp);
}

void TouchLcd::Internal_CGROM(void)
{
	uint8_t temp;
	WriteCommand(0x21);	//FNCR0
	temp = ReadData();
	temp &= 0xdf ;
	WriteData(temp);
}

void TouchLcd::Text_color(uint32_t color)
{
	#ifdef Color65k
	WriteCommand(0x63);
	WriteData((color&0xf800)>>11);

	WriteCommand(0x64);
	WriteData((color&0x07e0)>>5);

	WriteCommand(0x65);
	WriteData((color&0x001f));
	#endif

	#ifdef Color256
	WriteCommand(0x63);
	WriteData((color&0xf800)>>13);

	WriteCommand(0x64);
	WriteData((color&0x07e0)>>8);

	WriteCommand(0x65);
	WriteData((color&0x001f)>>3);
	#endif
}

void TouchLcd::Memory_Clear(void)
{
	uint8_t temp;
	WriteCommand(0x8e);
	temp = ReadData();
	temp |= 0x80;
	WriteCommand(0x8e);
	WriteData(temp);
	Chk_Busy();
}

void TouchLcd::Chk_Busy(void)
{
	do
	{
	}while((LCD_StatusRead() & 0x80) == 0x80);

}

uint8_t TouchLcd::LCD_StatusRead(void)
{
	uint8_t Cmd;
	digitalWrite(m_css, LOW);
	SPI.transfer(0xc0);
	Cmd = SPI.transfer(0x00);
	//SPI.transfer(0x00);
	//Cmd = SPDR;
	digitalWrite(m_css, HIGH);

	return Cmd;
}

uint8_t TouchLcd::ReadData(void)
{
	uint8_t Data;
	digitalWrite(m_css, LOW);
	SPI.transfer(0x40);
	Data = SPI.transfer(0x00);
	//SPI.transfer(0x00);
	//Data = SPDR;
	digitalWrite(m_css, HIGH);

	return Data;
}


void TouchLcd::Font_Extension(uint8_t vext, uint8_t hext)
{
	uint8_t temp;
	WriteCommand(0x22);
	temp = ReadData();
	temp &= ~(0xf);
	temp |= (hext | vext);
	WriteCommand(0x22);
	WriteData(temp);
	Chk_Busy();
}

void TouchLcd::LCD_DisplayOn(void)
{
	WriteCommand(0x01);
	WriteData(0x80);
}

void TouchLcd::WriteCommand(unsigned char cmd)
{
	digitalWrite(m_css, LOW);
	SPI.transfer(0x80);
	SPI.transfer(cmd);
	digitalWrite(m_css, HIGH);
}

void TouchLcd::WriteData(unsigned char data)
{
	digitalWrite(m_css, LOW);
	SPI.transfer(0x00);
	SPI.transfer(data);
	digitalWrite(m_css, HIGH);
}
