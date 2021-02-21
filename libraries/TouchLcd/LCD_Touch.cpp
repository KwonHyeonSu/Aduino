#include "TouchLcd.h"

//Touch touch;

#define XMIN 400
#define YMIN 25
#define XMAX 978
#define YMAX 240
#define LCD_X_SIZE 480
#define LCD_Y_SIZE 272
#define Xa  183
#define Ya  826	//LCD_Y_SIZE *7/8
#define Xb  871
#define Yb  177	//LCD_Y_SIZE *1/8

void TouchLcd::Touch_Initial() {

	Touch_Auto_Mode();
	Touch_ADC_Clock();
	Touch_Enable_TP();
}

void TouchLcd::Touch_Auto_Mode(void)
{
	unsigned char temp;
	SPI.transfer(0x71);
	temp = ReadData();
	temp &= 0xbf;
	WriteData(temp);
}

void TouchLcd::Touch_ADC_Clock(void)
{
	WriteCommand(0x70);
	WriteData(0xb2);
}

void TouchLcd::Touch_Enable_TP(void)
{
	unsigned char temp;
	WriteCommand(0x70);
	temp = ReadData();
	temp &= 0x80;
	WriteData(temp);

	ADC_value_X = ADC_value_Y = 0;
}

uint8_t TouchLcd::Touch_Detect(void)
{
	unsigned char h = 0;
	unsigned int i = 0, j = 0, ii = 0, jj = 0;

	unsigned int LCD_X_temp = ADC_value_X;
	unsigned int LCD_Y_temp = ADC_value_Y;
	static unsigned int counter1 = 0;
	static unsigned int counter2 = 0;

	clear_TP_interrupt();
	delay(1);

	if(Touch_Status())
	{
		if(counter1 == 0)
		{
			counter1++;
			h = 0;

			do{
				Access_TP_coordinate();
				i = ADC_value_X;
				j = ADC_value_Y;
				Access_TP_coordinate();
				ii = ADC_value_X;
				jj = ADC_value_Y;

				if((i == ii) && (j == jj))
					h++;
			}while((h < 1) && Touch_Status());

			LCD_X_temp = ADC_value_X;
			LCD_Y_temp = ADC_value_Y;
		}
		else
		{
			Access_TP_coordinate();

			if((ADC_value_X >= (LCD_X_temp + 8)) || (ADC_value_X <= (LCD_X_temp - 8)))
			{
				ADC_value_X = LCD_X_temp;
				ADC_value_Y = LCD_Y_temp;
			}
			else
			{
				counter2++;
			}

			if((ADC_value_Y >= (LCD_Y_temp + 8)) || (ADC_value_Y <= (LCD_Y_temp - 8)))
			{
				ADC_value_X = LCD_X_temp;
				ADC_value_Y = LCD_Y_temp;
			}

			if(counter2 > 10)
			{
				counter1 = 0;
				counter2 = 0;
			}
		}
		return 1;
	}
	else
	{
		ADC_value_X = 0;
		ADC_value_Y = 0;
		counter1 = 0;
	}

	return 0;
}

#if 0
void TouchLcd::Trace_XY_Value(void)
{
	unsigned char temp;
	unsigned int temp1;

	temp1 = ADC_value_X;
	temp = (ADC_value_X >> 8);
	Font_Coordinate(0,20);
	Show_String("X=0x",0);
	Print_Hex(temp);
	temp = temp1;
	Print_Hex(temp);
	Chk_Busy();

	temp1 = ADC_value_Y;
	temp = (ADC_value_Y >> 8);
	Font_Coordinate(0, 60);
	Show_String("Y=0x",0);
	Print_Hex(temp);
	temp = temp1;
	Print_Hex(temp);
	Chk_Busy();
}

void TouchLcd::Print_Hex(unsigned char buf)
{
	unsigned char temp;
	temp = buf;
	temp = (temp >> 4) & 0x0F;
	if(temp < 0x0A)
	{
		temp |= 0x30;
	}
	else
		temp = temp + 0x37;

	WriteData(temp);
	Chk_Busy();

	temp = buf;
	temp = temp & 0x0F;
	if(temp < 0x0A)
	{
		temp |= 0x30;
	}
	else
		temp = temp + 0x37;

	WriteData(temp);
	Chk_Busy();
}

void TouchLcd::Print_Decimal(unsigned int buffer)
{
	unsigned int temp,temp1;

	temp = buffer/100;
	temp1= buffer%100;

	temp |= 0x30;
	WriteData(temp);
	Chk_Busy();

	temp = temp1/10;
	temp |= 0x30;
	WriteData(temp);
	Chk_Busy();

	temp = temp1%10;
	temp |= 0x30;
	WriteData(temp);
	Chk_Busy();
}
#endif

uint32_t TouchLcd::GetXYValue(uint8_t kind_xy)
{
	uint32_t temp_xy = 0;

	switch(kind_xy)
	{
		case POS_X:
			temp_xy = map(ADC_value_X, 0, 1023, 0, LCD_X_SIZE);
			//Serial.print("temp x : ");
			//Serial.print(temp_xy);
			temp_xy = map(temp_xy, 25, 460, 0, LCD_X_SIZE);
			temp_xy = constrain(temp_xy, 0, LCD_X_SIZE);
			break;

		case POS_Y:
			temp_xy = map(ADC_value_Y, 0, 1023, 0, LCD_Y_SIZE);
			//Serial.print("\t\ttemp y : ");
			//Serial.println(temp_xy);
			temp_xy = map(temp_xy, 35, 245, 0, LCD_Y_SIZE);
			temp_xy = constrain(temp_xy, 0, LCD_Y_SIZE);

			break;
	}

	return temp_xy;
}

#if 0
void TouchLcd::Trace_coordinate(void)
{
	Font_Coordinate(0,100);
	Show_String("LCD_X=",0);
	//Print_Decimal(LCD_X);
	Print_Decimal(map(ADC_value_X, 0, 1023, 0, LCD_X_SIZE));

	Font_Coordinate(0,140);
	Show_String("LCD_Y=",0);
	//Print_Decimal(LCD_Y);
	Print_Decimal(map(ADC_value_Y, 0, 1023, 0, LCD_Y_SIZE));
}
#endif

void TouchLcd::Access_TP_coordinate(void)
{
	clear_TP_interrupt();
	delay(1);

	if(Touch_Status())
	{
		ADC_value_X = Get_X_ADC_Value_10bit();
		ADC_value_Y= Get_Y_ADC_Value_10bit();

		clear_TP_interrupt();
		delay(1);
	}
	/*
	else
	{
		ADC_value_X = ADC_value_Y = 0;
	}
	*/
}

#if 0
void TouchLcd::Get_XY_Calibration(void)
{
	long ttemp;
	long temp1;

	temp1 = ADC_value_X;

	if (temp1<(XMIN-OFFSET_X))
	{
		temp1=(XMIN-OFFSET_X);
	}

	if (temp1>(XMAX-OFFSET_X))
	{
		temp1=(XMAX-OFFSET_X);
	}

	ttemp = temp1-(XMIN-OFFSET_X);
	ttemp *= LCD_X_SIZE;
	ttemp /= (XMAX-XMIN);
	LCD_X = (unsigned int) ttemp;


	temp1 = ADC_value_Y;

	if (temp1<(YMIN-OFFSET_Y))
	{
		temp1=(YMIN-OFFSET_Y);
	}

	if (temp1>(YMAX-OFFSET_Y))
	{
		temp1=(YMAX-OFFSET_Y);
	}

	ttemp = temp1-(YMIN-OFFSET_Y);
	ttemp *= LCD_Y_SIZE;
	ttemp /= (YMAX-YMIN);
	LCD_Y = (unsigned int) ttemp;
}

void TouchLcd::MidValueCalculate(void)
{
	if (X2>X3)
	{
		if (X3>X4)
		{
			X1=X3;
		}
		else
		{
			if (X2>X4)
			{
				X1=X4;
			}
			else
			{
				X1=X2;
			}
		}
	}
	else
	{
		if (X3<X4)
		{
			X1=X3;
		}
		else
		{
			if (X2<X4)
			{
				X1=X4;
			}
			else
			{
				X1=X2;
			}
		}
	}

	if (Y2>Y3)
	{
		if (Y3>Y4)
		{
			Y1=Y3;
		}
		else
		{
			if (Y2>Y4)
			{
				Y1=Y4;
			}
			else
			{
				Y1=Y2;
			}
		}
	}
	else
	{
		if (Y3<Y4)
		{
			Y1=Y3;
		}
		else
		{
			if (Y2<Y4)
			{
				Y1=Y4;
			}
			else
			{
				Y1=Y2;
			}
		}
	}

	#ifdef TP_XY_exchange
		ADC_value_X = Y1;
		ADC_value_Y = X1;
	#else
		ADC_value_X = X1;
		ADC_value_Y = Y1;
	#endif

	#ifdef TP_X_swap
		ADC_value_X = 0x03ff-ADC_value_X;
	#endif

	#ifdef TP_Y_swap
		ADC_value_Y = 0x03ff-ADC_value_Y;
	#endif
 }
#endif

unsigned int TouchLcd::Get_X_ADC_Value_10bit(void)
{
	unsigned int temp,temp1;

	temp = 0;
	temp |= ADC_X();
	temp1 = (temp<<2);
	temp = temp1 |((ADC_XY() & 0x03));

	return temp;
}

unsigned int TouchLcd::Get_Y_ADC_Value_10bit(void)
{
	unsigned int temp,temp1;

	temp = 0;
	temp |= ADC_Y();
	temp1 = (temp<<2);
	temp = temp1 | ((ADC_XY()>>2)&0x03);

	return temp;
}

unsigned char TouchLcd::ADC_X(void)
{	unsigned char temp;
	WriteCommand(0x72);
	temp = ReadData();
	return temp;
}

unsigned char TouchLcd::ADC_Y(void)
{
	unsigned char temp;
	WriteCommand(0x73);
	temp = ReadData();
	return temp;
}

unsigned char TouchLcd::ADC_XY(void)
{
  	unsigned char temp;
 	WriteCommand(0x74);
	temp = ReadData();
	return temp;
}

unsigned char TouchLcd::Touch_Status(void)
{
	unsigned char temp;
	WriteCommand(0xf1);
	temp = ReadData();

	if((temp & 0x04) == 0x04)
		return 1;
	else
		return 0;
}

void TouchLcd::clear_TP_interrupt(void)
{
	unsigned char temp;
	WriteCommand(0xf1);
	temp = ReadData();
	temp |= 0x04;
	WriteData(temp);
}
