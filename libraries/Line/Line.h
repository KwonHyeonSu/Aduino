#ifndef __LINE_H__
#define __LINE_H__

#include <inttypes.h>

#define DEFAULT_TH	700

class Line
{
public:
	Line();
	void begin(uint8_t pinNum);
	void setThreshold(int th);
	int isLine();

private:
	int m_pinNum;
	int m_th;
};

#endif //__LINE_H__