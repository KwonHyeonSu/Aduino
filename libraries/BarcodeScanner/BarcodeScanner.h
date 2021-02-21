#ifndef __BARCODESCANNER_H__
#define __BARCODESCANNER_H__


#include <hidboot.h>

#define BS_BUFFER_SIZE 64

class BarcodeScanner : public KeyboardReportParser
{
        void PrintKey(uint8_t mod, uint8_t key);             // Add this line to print character in ASCII
protected:
	virtual void OnKeyDown	(uint8_t mod, uint8_t key);
	virtual void OnKeyPressed(uint8_t key);

	char inputBuffer[BS_BUFFER_SIZE];
	byte m_head;
	byte m_tail;

	void putBuffer(char c);

public:
	BarcodeScanner();
	void begin();
	void Task();
	int available(void);
	char read(void);
};

#endif // __BARCODESCANNER_H__