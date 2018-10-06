#include <Arduino.h>
#include <Servo.h>
#include "pins.h"
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
  
  arm.attach(ARM_SERVO);
  arm.write(ARM_UP);
  claw.attach(CLAW_SERVO);
  claw.write(CLAW_OPEN);
  dump.attach(DUMP_SERVO);
  dump.write(DONT_DUMP);

  
  racquetArm.attach(RACQUET_BALL_SERVO);
  racquetArm.write(PUSHER_UP);

}


void loop() {
  static int state = -1;
  readLine();
  if(digitalRead(BUTTON_2) == LOW) state = 0;
  if(state != 7) {
    racquetArm.write(PUSHER_UP);
  }
  switch(state) {

    case -1:
      if(displayMissionState()) state++;
      break;
    case 0:
      if(waitState())  state++;
      break;
    case 1:
      if(followRedPathState())  state++;
      break;
    case 2:
      if(depositPeopleState())  state++;
      break;
    case 3: 
    //reset
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
    case 6:
      if(followRacquetballState()) state++;
      break;
    case 7:
      if(depositRacquetballState()) state++;
      break;

  default:
      if(doneState()) state = 0;
      break;
  }


}
