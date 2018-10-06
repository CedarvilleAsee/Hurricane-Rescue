#ifndef SUBSTATES
#define SUBSTATES

// State 0
//  Waits until the button on board is pushed, go to next state
bool waitState() {
  //for centering robot
  display.sendNum(sensorCounter, 0);
  //keeps the robot still
  writeToWheels(0, 0);
  //when started, copy missions into globals
  if(digitalRead(BUTTON_1) == LOW) {
    //get mission number from switches
    if(missionNum == 1) {
      strncpy(redPath, RED_MISSION_1, RED_STEPS_1);
      strncpy(neutralPath, NEUTRAL_MISSION_1, NEUTRAL_STEPS_1);
      strncpy(redPickup, RED_PICKUP_1, RED_STEPS_1);
      strncpy(neutralPickup, NEUTRAL_PICKUP_1, NEUTRAL_STEPS_1);
      redSteps = RED_STEPS_1;
      neutralSteps = NEUTRAL_STEPS_1;
    }
    else if(missionNum == 2) {
      strncpy(redPath, RED_MISSION_2, RED_STEPS_2);
      strncpy(neutralPath, NEUTRAL_MISSION_2, NEUTRAL_STEPS_2);
      strncpy(redPickup, RED_PICKUP_2, RED_STEPS_2);
      strncpy(neutralPickup, NEUTRAL_PICKUP_2, NEUTRAL_STEPS_2);
      redSteps = RED_STEPS_2;
      neutralSteps = NEUTRAL_STEPS_2;
    }
    else if(missionNum == 3) {
      strncpy(redPath, RED_MISSION_3, RED_STEPS_3);
      strncpy(neutralPath, NEUTRAL_MISSION_3, NEUTRAL_STEPS_3);
      strncpy(redPickup, RED_PICKUP_3, RED_STEPS_3);
      strncpy(neutralPickup, NEUTRAL_PICKUP_3, NEUTRAL_STEPS_3);
      redSteps = RED_STEPS_3;
      neutralSteps = NEUTRAL_STEPS_3;
    }
    else if(missionNum == 4) {
      strncpy(redPath, RED_MISSION_4, RED_STEPS_4);
      strncpy(neutralPath, NEUTRAL_MISSION_4, NEUTRAL_STEPS_4);
      strncpy(redPickup, RED_PICKUP_4, RED_STEPS_4);
      strncpy(neutralPickup, NEUTRAL_PICKUP_4, NEUTRAL_STEPS_4);
      redSteps = RED_STEPS_4;
      neutralSteps = NEUTRAL_STEPS_4;
    }
    return true;
  }
  return false;
}


void doPickupSequence(const char sequence[], int pathIndex) {
  static int pickupStateIndex = 0;
  static bool atNextSquare = true;
  static int prevIndex = pathIndex;
  static bool clawClose = false;
  if(prevIndex != pathIndex) { 
    prevIndex = pathIndex;
    atNextSquare = true;
  }
  if(atNextSquare){
    if(sequence[pathIndex] == E) {
      display.sendMessage(PICKUP_EMPTY);
      atNextSquare = false;
      return;
    }
    else if(sequence[pathIndex] == L) {
      display.sendMessage(PICKUP_LEFT);
        atNextSquare = false; //temp
        return;//temp
      /*switch(pickupStateIndex) {
        case 0:
          leftClawArm.write(L_DOWN_POS);
          if(delayState(400)) pickupStateIndex++;
          break;
        case 1:
          if(readLeftClaw() < PERSON_CLOSE) {
            leftClaw.write(L_CLOSE);
            if(delayState(100)) pickupStateIndex++;
          }
          break;
        case 2:
          leftClawArm.write(L_UP_POS);
          if(delayState(400)) pickupStateIndex++;
          break;
        case 3:
          leftClaw.write(L_OPEN);
          if(delayState(100)) {
            pickupStateIndex = 0;
            atNextSquare = false;
            return;
          }
          break;
        default:
          leftClawArm.write(L_UP_POS);
          leftClaw.write(L_OPEN);
          break;
      }*/
    }
    else if(sequence[pathIndex] == R) {
      display.sendMessage(PICKUP_RIGHT);
      if(clawClose) claw.write(CLAW_CLOSE);
      switch(pickupStateIndex) {
        case 0:
          arm.write(ARM_DOWN);
          if(delayState(80))
          pickupStateIndex++;
          break;
        case 1:
          if(readClaw() < PERSON_CLOSE) {
            clawClose = true;
            if(delayState(100)) pickupStateIndex++;
          }
          break;
        case 2:
          arm.write(ARM_UP);
          if(delayState(500)) {
            clawClose = false;
            claw.write(CLAW_OPEN);
            pickupStateIndex++;
          }
          break;
        case 3:
          claw.write(CLAW_OPEN);
          if(delayState(10)){
            pickupStateIndex = 0;
            atNextSquare = false;
            return;
          }
          break;
      }
    }
  }
  return;
}

bool followRedPathState() {
  if(redIndex == redSteps) return true;
  if(doTurnSequence(redPath, redIndex, redSteps)) redIndex++;
  doPickupSequence(redPickup, pickupIndex);
  return false;
}

bool followNeutralPathState() {
  if(neutralIndex == neutralSteps) return true;
  if(doTurnSequence(neutralPath, neutralIndex, neutralSteps)) neutralIndex++;
  doPickupSequence(neutralPickup, pickupIndex);
  return false;
}

bool depositPeopleState(){
  static int depositIndex = 0;
  display.sendMessage(DEPOSITING);
  arm.write(ARM_MIDDLE);
  switch(depositIndex) {
    case 0:
      turn(HALF_SPEED, R);
      if(atIntersection()){
        writeToWheels(0, 0);
        depositIndex++;
      }
      break;
    case 1:
      dump.write(DO_DUMP);
      if(delayState(1500)) depositIndex++;
      break;
    case 2:
      dump.write(DONT_DUMP);
      if(turn(HALF_SPEED, R)) {
        depositIndex = 0;
        return true;
      }
      break;
  }
  return false;
}



bool followRacquetballState() {
  display.sendMessage(RACQUETBALL);
  if(racquetballIndex == RACQUET_BALL_STEPS) return true;
  if(doTurnSequence(RACQUET_BALL_PATH, racquetballIndex, RACQUET_BALL_STEPS)) racquetballIndex++;
  return false;
}

bool depositRacquetballState() {
  display.sendMessage(RACQUETBALL);
  static int subIndex = 0;
  switch(subIndex) {
    case 0:
      writeToWheels(-HALF_SPEED, HALF_SPEED);
      if(sensorsOnRight()) subIndex++;
      break;
    case 1:
      writeToWheels(0, 0);
      subIndex++;
      break;
    case 2:
      racquetArm.write(PUSHER_DOWN);
      if(delayState(700)) subIndex++;
      break;
    case 3:
      racquetArm.write(PUSHER_UP);
      if(delayState(200)) {
        subIndex = 0;
        return true;
      }
      break;
        
  }
  return false;
  
}

bool doneState() {
   display.sendMessage(DONE);
   writeToWheels(0, 0);
   return false;
}

bool displayMissionState() {
  display.sendNum(missionNum, 1);
  if(digitalRead(SWITCH1) == HIGH) {
    if(digitalRead(SWITCH0) == HIGH) {
      missionNum = 4;
    }
    else {
      missionNum = 3;
    }
  }
  else {
    if(digitalRead(SWITCH0) == HIGH) {
      missionNum = 2;
    }
    else {
      missionNum = 1;
    }
  }
  if(digitalRead(BUTTON_2) == LOW) {
    return true;
  }
  return false;
}

#endif

