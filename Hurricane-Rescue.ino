#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
#include "constants.h"
#include "missions.h"
#include "PT6961.h"
#include "globals.h"
#include "generalFunctions.h"
#include "display.h"
#include "lineFollowing.h"
#include "turning.h"
#include "subStates.h"


void setup() {
  oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  oledDisplay.display();
  oledDisplay.clearDisplay();
  oledDisplay.setTextSize(2);
  oledDisplay.setTextColor(WHITE);
  
  display.initDisplay();
  display.sendNum(1234, 1);
  // initialize line sensors
  for(int i = 0; i < 8; i++) {
    pinMode(LINE_SENSOR[i], INPUT);
  }
  

  pinMode(FRONT_SENSOR, INPUT);
  pinMode(FORK_SENSOR, INPUT);

 // initialize motor controllers
  pinMode(WHEEL_DIR_LB, OUTPUT);
  pinMode(WHEEL_DIR_LF, OUTPUT);
  pinMode(WHEEL_DIR_RB, OUTPUT);
  pinMode(WHEEL_DIR_RF, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);

  //pinMode(WHEEL_STBY, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);


  writeWheelDirection(WHEEL_FORWARDS, WHEEL_FORWARDS);

  rightArm.attach(ARM_SERVO_RIGHT);
  rightArm.write(RIGHT_ARM_START);
  
  leftArm.attach(ARM_SERVO_LEFT);
  leftArm.write(LEFT_ARM_START);
  
  rightClaw.attach(CLAW_SERVO_RIGHT);
  rightClaw.write(RIGHT_CLAW_OPEN);
  
  leftClaw.attach(CLAW_SERVO_LEFT);
  leftClaw.write(LEFT_CLAW_OPEN);
  
  dump.attach(DUMP_SERVO);
  dump.write(DONT_DUMP);
  
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY); //makes PB3 work
}

void loop() {
  static int state = -2;
  readLine();
  //if(digitalRead(BUTTON_2) == LOW) state = 0;
  switch(state) {
    case -2:
      if(pickMissionState()) state++;//press button2 to switch mission, button 1 to select
      break;
    case -1:
      if(displayMissionState()) state++; // button 2 to advance
    case 0:
      if(waitState()) state++; // button 1 to start
      break;
    case 1:
      if(followRedPathState())  state++;
      break;
    case 2:
      if(depositPeopleState())  state++;
      break;
    case 3: 
    //reset for second half
      pickupIndex = 0;
      atWall = false;
      state++;
      break;
    case 4:
      if(followNeutralPathState()) state++;
      break;
    case 5:
      if(depositPeopleState()) state++;
      break;

  default:
      if(doneState()) state = 0;
      break;
  }


}
