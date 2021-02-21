#include <PN532.h>
#include <SPI.h>
#include <String.h>

#define PN532_CS 77
PN532 nfc(PN532_CS);

void setup(void) {
  Serial.begin(9600);
  Serial.println("Hello!");
  nfc.begin( );
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    return ;
  }

  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);
  Serial.print("Supports "); 
  Serial.println(versiondata & 0xFF, HEX);

  nfc.SAMConfig( );
}

char DataOut[16]="USE"; //16bytes
char DataIn[16];//Should be 16bytes
String szDataIn= "";
String cmd = "";
String value = "";
boolean configFlag = true;
int count = 100;

void loop(void) {      
  if(nfc.configurePeerAsTarget()) 
  {
    DataOut[4] = (char)count;
    if( configFlag && nfc.targetTxRx(DataOut,DataIn))
    {
      DataCharToString();
      setData();
      Serial.print("cmd: ");
      Serial.println(cmd);
      Serial.print("value: ");
      Serial.println(value);
      Serial.print("count: ");
      Serial.println(count);      
    }
    configFlag = false;
  }
  configFlag = true;
}
void DataCharToString( )
{
  szDataIn = "";
  for(int i =0; i < 15 ; i++)
  {
    szDataIn  += DataIn[i];
  }
  cmd = szDataIn.substring(0,4);
  value = szDataIn.substring(4);
}

void setData( )
{
  if(cmd == "USE1")
  {
    count--;
  }else if(cmd == "USE2")
  {
    count += value.charAt(0); 
    Serial.println(value.charAt(0));
  }
}
