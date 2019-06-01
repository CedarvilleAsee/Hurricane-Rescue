#ifndef SUBSTATES
#define SUBSTATES

// State 0
//  Waits until the button on board is pushed, go to next state
bool waitState() {
  //for centering robot
  display.sendNum(sensorCounter, 0);
  printDebugging();
  //keeps the robot still
  writeToWheels(0, 0);
  //when started, copy missions into globals
  if (digitalRead(BUTTON_1) == LOW) {
    //get mission number from switches
    if (missionNum == 1) {
      strncpy(redPath, RED_MISSION_1, RED_STEPS_1);
      strncpy(neutralPath, NEUTRAL_MISSION_1, NEUTRAL_STEPS_1);
      strncpy(redPickup, RED_PICKUP_1, RED_STEPS_1);
      strncpy(neutralPickup, NEUTRAL_PICKUP_1, NEUTRAL_STEPS_1);
      redSteps = RED_STEPS_1;
      neutralSteps = NEUTRAL_STEPS_1;
    }
    else if (missionNum == 2) {
      strncpy(redPath, RED_MISSION_2, RED_STEPS_2);
      strncpy(neutralPath, NEUTRAL_MISSION_2, NEUTRAL_STEPS_2);
      strncpy(redPickup, RED_PICKUP_2, RED_STEPS_2);
      strncpy(neutralPickup, NEUTRAL_PICKUP_2, NEUTRAL_STEPS_2);
      redSteps = RED_STEPS_2;
      neutralSteps = NEUTRAL_STEPS_2;
    }
    else if (missionNum == 3) {
      strncpy(redPath, RED_MISSION_3, RED_STEPS_3);
      strncpy(neutralPath, NEUTRAL_MISSION_3, NEUTRAL_STEPS_3);
      strncpy(redPickup, RED_PICKUP_3, RED_STEPS_3);
      strncpy(neutralPickup, NEUTRAL_PICKUP_3, NEUTRAL_STEPS_3);
      redSteps = RED_STEPS_3;
      neutralSteps = NEUTRAL_STEPS_3;
    }
    else if (missionNum == 4) {
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

bool pickMissionState() {
  displayMission(missionNum);
  if (digitalRead(BUTTON_2) == LOW) {
    missionNum++;
    if (missionNum == 5) {
      missionNum = 1;
    }
    while (digitalRead(BUTTON_2) == LOW);
  }
  if (digitalRead(BUTTON_1) == LOW) {
    while (digitalRead(BUTTON_1) == LOW);
    return true;
  }
  displayMission(missionNum);
  return false;
}

bool displayMissionState() {
  if (digitalRead(BUTTON_2) == LOW) {
    while (digitalRead(BUTTON_2) == LOW);
    return true;
  }
  displayMissionConfirmation(missionNum);
  return false;
}


bool doPickupSequence(const char sequence[], int pathIndex) {
  static int pickupStateIndex = 0;
  static int currentIndex = pathIndex;
  static bool inProgress = true;
  
  if (!inProgress && currentIndex != pathIndex) {
    currentIndex = pathIndex;
    atNextLine = false;
    inProgress = true;
  }

  if (inProgress) {
    if (sequence[currentIndex] == E) {
      //display.sendMessage(PICKUP_EMPTY);
      inProgress = false;
      return true;
    }
    else if (sequence[currentIndex] == L) {
      static int pickupStateIndex = -1;
      //display.sendMessage(PICKUP_LEFT);
      switch (pickupStateIndex) {
        case -1:
          rightArm.write(RIGHT_ARM_WAIT);
          if (delayState(20)) {
            pickupStateIndex++;
          }
          break;
        case 0:
          leftArm.write(LEFT_ARM_DOWN);
          leftClaw.write(LEFT_CLAW_OPEN);
          if (delayState(20))
            pickupStateIndex++;
          break;
        case 1:
          if (readLeftClaw() < PERSON_CLOSE_LEFT) {
            leftClaw.write(LEFT_CLAW_CLOSE);
            pickupStateIndex++;
          }
          if (atNextLine) {
            pickupStateIndex = -1;
            inProgress = false;
            leftClaw.write(LEFT_CLAW_CLOSE);
            leftArm.write(LEFT_ARM_WAIT);
            return true;
          }
          break;
        case 2:
          if (delayState(250)) pickupStateIndex++;
          break;
        case 3:
          leftArm.write(LEFT_ARM_UP);
          if (delayState(250)) {
            leftClaw.write(LEFT_CLAW_OPEN);
            pickupStateIndex++;
          }
          break;
        case 4:
          if (delayState(100)) {
            leftClaw.write(LEFT_CLAW_CLOSE);
            pickupStateIndex++;
          }
          break;
        case 5:
          leftClaw.write(LEFT_CLAW_CLOSE);
          if (delayState(50)) {
            pickupStateIndex = -1;
            inProgress = false;
            return true;
          }
          break;
      }
    }
    else if (sequence[currentIndex] == R) {
      static int pickupStateIndex = -1;
      switch (pickupStateIndex) {
        case -1:
          leftArm.write(LEFT_ARM_WAIT);
          if (delayState(20)) {
            pickupStateIndex++;
          }
          break;
        case 0:
          rightArm.write(RIGHT_ARM_DOWN);
          rightClaw.write(RIGHT_CLAW_OPEN);
          if (delayState(20))
            pickupStateIndex++;
          break;
        case 1:
          if (readRightClaw() < PERSON_CLOSE_RIGHT) {
            rightClaw.write(RIGHT_CLAW_CLOSE);
            pickupStateIndex++;
          }
          if (atNextLine) {
            pickupStateIndex = -1;
            inProgress = false;
            rightClaw.write(RIGHT_CLAW_CLOSE);
            rightArm.write(RIGHT_ARM_WAIT);
            return true;
          }
          break;
        case 2:
          if (delayState(250)) pickupStateIndex++;
          break;
        case 3:
          rightArm.write(RIGHT_ARM_UP);
          if (delayState(250)) {
            rightClaw.write(RIGHT_CLAW_OPEN);
            pickupStateIndex++;
          }
          break;
        case 4:
          if (delayState(100)) {
            pickupStateIndex++;
            rightClaw.write(RIGHT_CLAW_CLOSE);
          }
          break;
        case 5:
          rightClaw.write(RIGHT_CLAW_CLOSE);
          if (delayState(50)) {
            pickupStateIndex = -1;
            inProgress = false;
            return true;
          }
          break;
      }
    }
  }
  return false;
}

bool followRedPathState() {
  int goFast = 0;
  if (redIndex == redSteps) return true;
  if (redPickup[redIndex] == E) {
    //goFast = 1;
    if ((redIndex + 1 < redSteps) && (redPath[redIndex + 1] == F) && redPath[redIndex]== F ) {
      goFast = 1;
    }
  }

  if (doTurnSequence(redPath, redIndex, redSteps, goFast)) {
    mil = millis();
    redIndex++;
  }
  if (pickupIndex != redIndex) {
    if (delayState(50)) {
      pickupIndex = redIndex;
    }
  }
  display.sendNum(redIndex, 0);
  doPickupSequence(redPickup, pickupIndex);
  return false;
}

bool followNeutralPathState() {
  int goFast = 0;
  if (neutralIndex == neutralSteps) return true;
  if (neutralIndex == neutralSteps - 2) {
    leftArm.write(LEFT_ARM_START);
    rightArm.write(RIGHT_ARM_START);
  }
  if (neutralPickup[neutralIndex] == E) {
    //goFast = 1;
    if ((neutralIndex + 1 < neutralSteps) && (neutralPath[neutralIndex + 1] == F) && neutralPath[neutralIndex] == F) {
      goFast = 1;
    }
  }
  if (doTurnSequence(neutralPath, neutralIndex, neutralSteps, goFast)) {
    mil = millis();
    neutralIndex++;
  }
  display.sendNum(neutralIndex, 0);
  if (pickupIndex != neutralIndex) {
    if (delayState(50)) {
      pickupIndex = neutralIndex;
    }
  }
  doPickupSequence(neutralPickup, pickupIndex);
  return false;
}

bool depositPeopleState() {
  static int depositIndex = 0;
  //display.sendMessage(DEPOSITING);
  switch (depositIndex) {
    case 0:
      turn(TURN_SPEED, R);
      if (sensorCounter == 192) {
        writeToWheels(0, 0);
        depositIndex++;
      }
      break;
    case 1:
      dump.write(DO_DUMP);
      if (delayState(1000)) depositIndex++;
      break;
    case 2:
      dump.write(DONT_DUMP);
      if (turn(TURN_SPEED, R)) {
        depositIndex = 0;
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



#endif
