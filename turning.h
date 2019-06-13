#ifndef TURNING
#define TURNING


//fast - do turn sequence

bool turn(int spd, char dir) {
  static int lineCount = 0;
  static bool gotOffLine = false;

  if (dir == F) {
    return true;
  }
  if (dir == L) {
    writeToWheels(-spd, spd);
  }
  else if (dir == R || atWall) {
    writeToWheels(spd, -spd);
  }
  if (sensors[2] == LOW && sensors[3] == LOW && sensors[4] == LOW && sensors[5] ==  LOW) gotOffLine = true;
  if (dir == B && !atWall) {
    writeToWheels(-(spd), spd);
    if (sensorsCentered() && gotOffLine) {
      // if it isn't at a wall,
      //the line sensors have to pass another line to turn completely around
      lineCount++;
      gotOffLine = false;
    }
    if (lineCount == 2) {
      lineCount = 0;
      return true;
    }
  }
  // Return true if the robot is back centered on the line

  if (gotOffLine && sensorsCentered()) {
    atWall = false;
    gotOffLine = false;
    return true;
  }
  return false;

}

bool doTurnSequence(const char sequence[], int index, int maxSteps, int redFast = 0) {
  //int speedVal = FULL_SPEED;
  //int turnStrictness = LINE_STRICTNESS;
  /*if(redFast == 1){
    speedVal = RED_FAST_SPEED;
    turnStrictness = RED_FAST_STRICTNESS;
  }
  else if (redFast == 2){
    speedVal = RED_REALLY_FAST_SPEED;
    turnStrictness = RED_REALLY_FAST_STRICTNESS;
  }*/
  int temp = index + 1; // should be acting on next instruction, not current
  if (temp == maxSteps) {
    if (lineFollow(FULL_SPEED, LINE_STRICTNESS)) {
      return true;
    }
  }

  else {
    if (turning) {
      if (turn(TURN_SPEED, sequence[temp])) {
        turning = false;
        return true;
      }
    }
    else {
      turning = lineFollow(FULL_SPEED, LINE_STRICTNESS);
    }
  }
  return false;
}


#endif
