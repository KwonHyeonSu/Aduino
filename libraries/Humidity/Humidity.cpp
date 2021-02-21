#include "Humidity.h"
#include "Arduino.h"

/*
Humidity::Humidity(int pinNum) {
	humPin = pinNum;

	begin();
}
*/

void Humidity::begin(int pinNum) {
	humPin = pinNum;
	pinMode(humPin, INPUT);
}

float Humidity::read(float temp) {
	return getHumidity(temp);
}

float Humidity::getHumidity(float temp){
   //caculate relative humidity
   float supplyVolt = 5.0;
 
   // read the value from the sensor:
   int HIH4030_Value = analogRead(humPin);
   float voltage = HIH4030_Value/1023. * supplyVolt; // convert to voltage value
 
   // convert the voltage to a relative humidity
   // - the equation is derived from the HIH-4030/31 datasheet
   // - it is not calibrated to your individual sensor
   //  Table 2 of the sheet shows the may deviate from this line
   float sensorRH = 161.0 * voltage / supplyVolt - 25.8;
   float trueRH = sensorRH / (1.0546 - 0.00216 * temp); //temperature adjustment
  
   return trueRH;
 }

Humidity gHumidity;