/* joystick_lib.ino */

#include "Joystick.h"

#define JOY_X  68
#define JOY_Y  69
#define JOY_SEL  9

Joystick joystick;

void setup() {
  joystick.begin(JOY_X, JOY_Y, JOY_SEL);
  joystick.setCenterPos();

  Serial.begin(115200);
}

void loop() {
  int xPos = joystick.getXPos();
  int yPos = joystick.getYPos();
  int isPush = joystick.isPushKey();

  if(xPos < 0) {
    Serial.print("LEFT");
  } else if(xPos > 0) {
    Serial.print("RIGHT");
  } else {
    Serial.print("CENTER");
  }

  Serial.print(" ");

  if(yPos < 0) {
    Serial.print("UP");
  } else if(yPos > 0) {
    Serial.print("DOWN");
  } else {
    Serial.print("CENTER");
  }
  
  Serial.print(" ");

  if(isPush == 1) {
    Serial.println("Button Pushed");
  } else {
    Serial.println();
  }

  delay(500);
}

