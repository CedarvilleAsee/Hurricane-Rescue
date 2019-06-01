#ifndef SUBSTATES
#define SUBSTATES

// State 0
//  Waits until the button on board is pushed, go to next state
bool waitState() {
  //for centering robot
  printDebugging();
  display.sendNum(sensorCounter);

  //keeps the robot still
  writeToWheels(0, 0);
  //when started, copy missions into globals
  if (digitalRead(BUTTON_1) == LOW) {
    return true;
  }
  return false;
} //change to base off of distance sensor

bool waitStateStart() {

  //wait until the other robot leaves to start
  if (robotGone()) {
    printNum(5);
    delay(1000);
    printNum(4);
    delay(1000);
    printNum(3);
    delay(1000);
    printNum(2);
    delay(1000);
    printNum(1);
    delay(1000);
    displayText("Going to  position");
    return true;
  }
  return false;
}


bool waitStateReturn() {
  //wait 90sec or until one return
  writeToWheels(0, 0);
  int temp = millis() + 85000;
  while (temp > millis()) {
    //trip proximity: exit true
    if (robotClose()) {
      while (!robotGone());
      printNum(5);
      delay(1000);
      printNum(4);
      delay(1000);
      printNum(3);
      delay(1000);
      printNum(2);
      delay(1000);
      printNum(1);
      delay(1000);
      displayText("Going to  fire");
      return true;
    }
  }
  ranOutOfTime = true;
  return true;

  //if timer runs down
} //make so waits for other robot to return or timeout


bool moveToFire() {
  static bool hasModdedPath = false;
  if (racquetballIndex == RACQUET_BALL_STEPS) return true;
  if (doTurnSequence(currentPath, racquetballIndex, RACQUET_BALL_STEPS)) {
    mil = millis();

    racquetballIndex++;
    int temp = racquetballIndex + 1;
    if (ranOutOfTime && !hasModdedPath && temp <= 6 && temp >= 2 && robotObstruct()) { //seems to drive drunkenly
      currentPath[7] = F;
      currentPath[8] = F;
      currentPath[temp] = L;
      currentPath[temp + 1] = R;
      displayText("Obstacle");
      hasModdedPath = true;
    }

  }
  if (atIntersection()) {
    display.sendNum(racquetballIndex + 100, 0);
  }
  else {
    display.sendNum(racquetballIndex, 0);
  }
  return false;
}

bool putOutFire() { //change so moves further at end to reach fire
  writeToWheels(0, 0);
  static int depositIndex = 0;
  display.sendMessage(DEPOSITING);
  for (int i = DONT_DUMP; i > DO_DUMP; i--) {
    dump.write(i);
    delay(15);
  }
  delay(2500);
  for (int i = DO_DUMP; i < DONT_DUMP; i++) {
    dump.write(i);
    delay(5);
  }
  return true;
}

bool doneState() {
  display.sendMessage(DONE);
  writeToWheels(0, 0);
  return false;
}



#endif
