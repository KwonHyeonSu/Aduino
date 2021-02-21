
#include <BarcodeScanner.h>

#include <Usb.h>
#include <usbhub.h>
#include <avr/pgmspace.h>
#include <hidboot.h>
#include <hiduniversal.h>
#include <BarcodeScanner.h>

USB     Usb;
USBHub     Hub(&Usb);                                          //I enable this line
HIDUniversal      Hid(&Usb);                                  //Add this line so that the barcode scanner will be recognized, I use "Hid" below 
HIDBoot<HID_PROTOCOL_KEYBOARD>    Keyboard(&Usb);

BarcodeScanner::BarcodeScanner() 
{
	m_tail = m_head = 0;

}

void BarcodeScanner::begin()
{
	delay(200);

    if (Usb.Init() == -1) {
        Serial.println("OSC did not start.");
    }
 
    delay( 200 );
 
    Hid.SetReportParser(0, (HIDReportParser*)this);        //Here I change  "Keyboard" for "Hid"
    Serial.println("Ready");
}

void BarcodeScanner::Task()
{
	Usb.Task();	
}

void BarcodeScanner::OnKeyDown(uint8_t mod, uint8_t key)	
{
    uint8_t c = OemToAscii(mod, key);
 
    if (c)
        OnKeyPressed(c);
}
 
/* what to do when symbol arrives */
void BarcodeScanner::OnKeyPressed(uint8_t key)	
{
static uint32_t next_time = 0;      //watchdog
static uint8_t current_cursor = 0;  //tracks current cursor position  
     //Serial.print( (char)key );      //Add char to print correct number in ASCII
	 putBuffer((char)key);
};
 

void BarcodeScanner::putBuffer(char c)
{
	byte i = (unsigned int)(m_head+1)%BS_BUFFER_SIZE;

	if(i != m_tail) {
		inputBuffer[m_head] = c;
		m_head = i;
	}
}

int BarcodeScanner::available(void)
{
	return ((unsigned int)(BS_BUFFER_SIZE + m_head - m_tail)) % BS_BUFFER_SIZE;
}

char BarcodeScanner::read(void)
{
	if(m_head == m_tail) return -1;
	else {
		unsigned char c = inputBuffer[m_tail];
		m_tail = (byte)(m_tail+1)%BS_BUFFER_SIZE;
		return c;
	}
}