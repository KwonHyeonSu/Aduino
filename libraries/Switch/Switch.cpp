/* GBTec_RnD 2012.03.26 Ver.01 */
/* Switch.cpp */

#include "Switch.h"
#include "Arduino.h"

#define SWITCH1_START	22
#define SWITCH2_START	30

void Switch::begin() {

	for(int i=SWITCH1_START; i<SWITCH1_START+8; i++) {
		pinMode(i, INPUT);
	}

	for(int i=SWITCH2_START; i<SWITCH2_START+8; i++) {
		pinMode(i, INPUT);
	}

	bPushKey = 0;
	pushKeyNum = 0;
}

uint8_t Switch::getPushKey() {
	uint8_t prePush=0;

	for(int i=SWITCH1_START; i<SWITCH1_START+8; i++) {
		if(digitalRead(i) == LOW) {
			prePush = i;
			break;
		}
	}

	for(int i=SWITCH2_START; i<SWITCH2_START+8; i++) {
		if(digitalRead(i) == LOW) {
			prePush = i;
			break;
		}
	}

	if(prePush != pushKeyNum) {
		pushKeyNum = prePush;
		return changePushKey(prePush);
	} else {
		return 0;
	}
}

uint8_t Switch::changePushKey(uint8_t key) {
	uint8_t keyNum=0;

	if(key == 0) return 0;

	switch(key) {
		case SWITCH1_START:		keyNum = 1;		break;
		case SWITCH1_START+1:	keyNum = 5;		break;
		case SWITCH1_START+2:	keyNum = 9;		break;
		case SWITCH1_START+3:	keyNum = 13;	break;
		case SWITCH1_START+4:	keyNum = 2;		break;
		case SWITCH1_START+5:	keyNum = 6;		break;
		case SWITCH1_START+6:	keyNum = 10;	break;
		case SWITCH1_START+7:	keyNum = 14;	break;

		case SWITCH2_START:		keyNum = 16;	break;
		case SWITCH2_START+1:	keyNum = 12;	break;
		case SWITCH2_START+2:	keyNum = 8;		break;
		case SWITCH2_START+3:	keyNum = 4;		break;
		case SWITCH2_START+4:	keyNum = 15;	break;
		case SWITCH2_START+5:	keyNum = 11;	break;
		case SWITCH2_START+6:	keyNum = 7;		break;
		case SWITCH2_START+7:	keyNum = 3;		break;
	}

//	pushKeyNum = 0;
	return keyNum;
}

uint8_t Switch::waitGetPushKey() {
	while(getPushKey() == 0) ;
	//return getPushKey();
	return changePushKey(pushKeyNum);
}

Switch gSwitch;