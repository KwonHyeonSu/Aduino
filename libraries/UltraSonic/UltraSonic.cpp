/* GBTec_RnD 2012. 3. 26 Ver.01 */
/* UltraSonic.cpp */

#include "UltraSonic.h"
#include "Arduino.h"


/*
UltraSonic::UltraSonic(int pinNum) {
	ultraPin = pinNum;
}
*/

void UltraSonic::begin(int pinNum) {
	ultraPin = pinNum;
	pinMode(ultraPin, INPUT);
}

int UltraSonic::ReadDistanceMilimeter() {
	double soundVelocity=0.0;
	double duration=0.0;
	
	soundVelocity = 331.5+(0.60714*25.0);
	duration = soundVelocity * (((double)getDuration()/1000000) / 2) * 1000;

	return (int)duration;
//	return getDuration() / 3 / 2;
}

int UltraSonic::ReadDistanceCentimeter() {
	double soundVelocity=0.0;
	double duration=0.0;
	
	soundVelocity = 331.5+(0.60714*25.0);
	duration = soundVelocity * (((double)getDuration()/1000000) / 2) * 100;

	return (int)duration;
	//return getDuration() / 29 / 2;  // 29 = (1/(331.5 + (0.60714*25.0)))*100
}

int UltraSonic::ReadDistanceInche() {
	//return getDuration() / 74/ 2;
	return (int)((float)ReadDistanceMilimeter() / 25.4);
}

uint32_t UltraSonic::getDuration() {
	uint32_t duration=0;

	if(ultraPin == 0) return 0;

	pinMode(ultraPin, OUTPUT);
	digitalWrite(ultraPin, LOW);
	delayMicroseconds(2);
	digitalWrite(ultraPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(ultraPin, LOW);

	pinMode(ultraPin, INPUT);
	duration = pulseIn(ultraPin, HIGH);

	//if(duration < 108) return 0;

	return duration;
}

UltraSonic gUltraSonic;