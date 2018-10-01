#include <Arduino.h>
#include <Servo.h>
#include "new_pins.h"
#include "constants.h"
#include "missions.h"
#include "PT6961.h"
#include "globals.h"
#include "generalFunctions.h"
#include "lineFollowing.h"
#include "turning.h"
#include "subStates.h"

void setup() {
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

  //pinMode(LEDY, OUTPUT);
  //pinMode(LEDG, OUTPUT);

  pinMode(WHEEL_SPEED_L, OUTPUT);
  pinMode(WHEEL_SPEED_R, OUTPUT);

  //pinMode(WHEEL_STBY, OUTPUT);

  pinMode(BUTTON_1, INPUT_PULLUP);


  writeWheelDirection(WHEEL_FORWARDS, WHEEL_FORWARDS);

  dump.attach(DUMP_SERVO);//dump bin
  dump.write(15);//to closed position
  eject.attach(EJECT_SERVO);
  eject.write(115);
  
  arm.attach(ARM_SERVO);
  arm.write(ARM_UP);
  claw.attach(CLAW_SERVO);
  claw.write(CLAW_OPEN);


}


void loop() {
  static int state = -1;
  readLine();
  if(digitalRead(BUTTON_2) == LOW) state = 0;
  switch(state) {
    case -1:
      if(displayMissionState()) state++;
      display.sendNum(missionNum, 1);
      break;
    case 0:
      if(waitState())  state++;
      display.sendNum(sensorCounter, 0);
      break;
    case 1:
      if(followRedPathState())  state++;
      break;
    case 2:
      pickupIndex = 0;
      if(depositPeopleState())  state=6;
      break;
    case 3: 
      if(turnAroundState()) state++;
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
